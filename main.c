#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include <omp.h>

#include "cpuinfo.h"
#include "wheel.h"
#include "sieve.h"

extern const uint8_t mask_table[8][8];
extern const uint8_t offset_table[8][8];

// Simple odds-only sieve to get our base primes up to sqrt(N)
uint32_t* generate_base_primes(uint64_t max_val, size_t* out_count) {
    uint64_t limit = (uint64_t)sqrt((double)max_val);
    if (limit < 2) { 
        *out_count = 0; 
        return NULL; 
    }
    size_t byte_array_size = (limit - 1) / 2;
    uint8_t* is_composite = (uint8_t*)calloc(byte_array_size, sizeof(uint8_t));
    if (!is_composite) return NULL;    
    size_t prime_count = 1; // Start with 2
    for (size_t i = 0; i < byte_array_size; i++) {
        if (!is_composite[i]) {
            prime_count++;
            uint64_t p = 2 * i + 3;
            uint64_t p_squared_idx = 2 * i * i + 6 * i + 3;
            for (size_t j = p_squared_idx; j < byte_array_size; j += p) {
                is_composite[j] = 1;
            }
        }
    }
    uint32_t* primes = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    primes[0] = 2;
    size_t write_idx = 1;
    for (size_t i = 0; i < byte_array_size; i++) {
        if (!is_composite[i]) {
            primes[write_idx++] = (uint32_t)(2 * i + 3);
        }
    }
    free(is_composite);
    *out_count = prime_count;
    return primes;
}

// Declare the 8 generated batch-processing functions
extern void process_residue1(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict primes, uint32_t count);
extern void process_residue7(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict primes, uint32_t count);
extern void process_residue11(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict primes, uint32_t count);
extern void process_residue13(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict primes, uint32_t count);
extern void process_residue17(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict primes, uint32_t count);
extern void process_residue19(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict primes, uint32_t count);
extern void process_residue23(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict primes, uint32_t count);
extern void process_residue29(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict primes, uint32_t count);

int main(int argc, char** argv) {
    uint64_t limit = 10000000000ULL; 
    if (argc > 1) {
        limit = strtoull(argv[1], NULL, 10);
    }

    setlocale(LC_ALL, "");    
    printf("Sieving to: %'lu\n", limit);
    double start_time = omp_get_wtime();

    size_t l1_cache_size = get_min_cache_size(1);
    
    // Force segment_bytes down to the nearest Power of 2 to eliminate division
    uint64_t segment_bytes = 1;
    while (segment_bytes * 2 <= l1_cache_size) {
        segment_bytes *= 2;
    }
    
    // Precalculate the bitwise shift and mask values for EratMedium routing
    uint32_t seg_shift = __builtin_ctz(segment_bytes); 
    uint32_t seg_mask = segment_bytes - 1;             
    uint64_t numbers_per_segment = segment_bytes * 30; 
    uint64_t total_segments = (limit / numbers_per_segment) + 1;

    size_t base_prime_count = 0;
    uint32_t* base_primes = generate_base_primes(limit, &base_prime_count);

    uint64_t total_primes = (limit >= 5) ? 3 : 0; 
    
    // Small primes must jump less than half the segment to guarantee multiple hits
    uint32_t small_prime_threshold = segment_bytes * 15;

    int num_threads = omp_get_max_threads();
    printf("Segment size: %'lu\nThreads: %u\n", segment_bytes, num_threads);
    uint64_t segments_per_thread = (total_segments + num_threads - 1) / num_threads;

    #pragma omp parallel reduction(+:total_primes)
    {
        int thread_id = omp_get_thread_num();
        uint64_t start_seg = thread_id * segments_per_thread;
        uint64_t end_seg = start_seg + segments_per_thread;
        if (end_seg > total_segments) end_seg = total_segments;
        
        if (start_seg < total_segments) {
            uint64_t thread_segment_count = end_seg - start_seg;
            SieveSegment* seg = create_segment(segment_bytes);
            
            // Allocate 8 separate arrays for the small primes
            SievingPrime* small_primes[8];
            uint32_t small_counts[8] = {0};
            for (int i = 0; i < 8; i++) {
                small_primes[i] = (SievingPrime*)malloc(base_prime_count * sizeof(SievingPrime));
            }

            BucketList* medium_buckets = (BucketList*)calloc(thread_segment_count, sizeof(BucketList));
            
            // Reduced pool size to prevent 100 Billion limit Segfaults
            BucketPool* pool = create_bucket_pool(1000); 
            if (!pool) {
                fprintf(stderr, "Thread %d: Failed to allocate bucket pool.\n", thread_id);
                exit(EXIT_FAILURE);
            }
            
            size_t next_base_prime_idx = 0;

            // Skip 2, 3, and 5
            while (next_base_prime_idx < base_prime_count && base_primes[next_base_prime_idx] <= 5) {
                next_base_prime_idx++;
            }

            for (uint64_t seg_idx = start_seg; seg_idx < end_seg; seg_idx++) {
                uint64_t absolute_seg_start = seg_idx * numbers_per_segment;
                uint64_t absolute_seg_end = absolute_seg_start + numbers_per_segment;
                uint64_t local_seg_idx = seg_idx - start_seg;

                // 1. ACTIVATE PENDING PRIMES AND FORK THE ROAD
                while (next_base_prime_idx < base_prime_count) {
                    uint32_t p = base_primes[next_base_prime_idx];
                    uint64_t p_squared = (uint64_t)p * p;
                    
                    if (p_squared >= absolute_seg_end) break; 

                    uint8_t k_residue = 0;
                    uint64_t first_multiple = calculate_first_valid_multiple(p, absolute_seg_start, &k_residue);
                    uint64_t offset = first_multiple - absolute_seg_start;
                    
                    SievingPrime sp;
                    sp.prime_k = p / 30;
                    sp.prime_bit_idx = map_remainder_to_bit_idx(p % 30);
                    sp.wheel_index = map_remainder_to_bit_idx(k_residue);
                    
                    if (p <= small_prime_threshold) {
                        // EratSmall: Route directly to its specific residue array
                        sp.byte_index = offset / 30;
                        small_primes[sp.prime_bit_idx][small_counts[sp.prime_bit_idx]++] = sp;
                    } else {
                        // EratMedium: Calculate future segment and push to bucket using bitwise math
                        uint64_t relative_start = first_multiple - (start_seg * numbers_per_segment);
                        uint32_t total_bytes = relative_start / 30;
                        uint64_t target_local_seg = total_bytes >> seg_shift;
                        
                        if (target_local_seg < thread_segment_count) {
                            sp.byte_index = total_bytes & seg_mask;
                            push_to_bucket(&medium_buckets[target_local_seg], sp, pool);
                        }
                    }
                    next_base_prime_idx++;
                }

                memset(seg->array, 0xFF, segment_bytes); 
                
                // Fix the off-by-one error (1 is not a prime!)
                if (seg_idx == 0) seg->array[0] &= 0xFE; 

                // 2. ERAT SMALL (Batched direct execution!)
                process_residue1(seg->array, segment_bytes, small_primes[0], small_counts[0]);
                process_residue7(seg->array, segment_bytes, small_primes[1], small_counts[1]);
                process_residue11(seg->array, segment_bytes, small_primes[2], small_counts[2]);
                process_residue13(seg->array, segment_bytes, small_primes[3], small_counts[3]);
                process_residue17(seg->array, segment_bytes, small_primes[4], small_counts[4]);
                process_residue19(seg->array, segment_bytes, small_primes[5], small_counts[5]);
                process_residue23(seg->array, segment_bytes, small_primes[6], small_counts[6]);
                process_residue29(seg->array, segment_bytes, small_primes[7], small_counts[7]);

                // 3. ERAT MEDIUM (Branchless Bucket Logic + Zero-Latency Recycling)
                BucketNode* current_node = medium_buckets[local_seg_idx].head;
                while (current_node != NULL) {
                    for (uint32_t i = 0; i < current_node->count; i++) {
                        SievingPrime sp = current_node->primes[i];
                        
                        seg->array[sp.byte_index] &= mask_table[sp.prime_bit_idx][sp.wheel_index];
                        
                        uint32_t jump = (sp.prime_k * wheel_gaps[sp.wheel_index]) + offset_table[sp.prime_bit_idx][sp.wheel_index];
                        sp.byte_index += jump;
                        sp.wheel_index = (sp.wheel_index + 1) & 7;
                        
                        // Bitwise segment routing
                        uint32_t segments_jumped = sp.byte_index >> seg_shift;
                        uint64_t target_local_seg = local_seg_idx + segments_jumped;
                        
                        if (target_local_seg < thread_segment_count) {
                            sp.byte_index &= seg_mask; 
                            push_to_bucket(&medium_buckets[target_local_seg], sp, pool);
                        }
                    }
                    
                    BucketNode* next_node = current_node->next;
                    return_node_to_pool(pool, current_node);
                    current_node = next_node;
                }
                
                medium_buckets[local_seg_idx].head = NULL;

                // 4. MASK AND COUNT
                if (seg_idx == total_segments - 1) {
                    mask_last_segment(seg->array, segment_bytes, limit, absolute_seg_start);
                }
                total_primes += count_primes_fast(seg->array, segment_bytes);
            }

            for (int i = 0; i < 8; i++) {
                free(small_primes[i]);
            }
            free(medium_buckets);
            free_bucket_pool(pool);
            free_segment(seg);
        }
    }

    double end_time = omp_get_wtime();
    printf("Primes found: %'lu\n", total_primes);
    printf("Time elapsed: %f seconds\n", end_time - start_time);

    free(base_primes);
    return 0;
}
