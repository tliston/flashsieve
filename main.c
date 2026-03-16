#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include <omp.h>

#include "cpuinfo.h"
#include "sieve.h"

// Simple odds-only sieve to get our base primes up to sqrt(N)
uint32_t *generate_base_primes(uint64_t max_val, size_t *out_count) {
    uint64_t limit = (uint64_t) sqrt((double)max_val);
    if (limit < 2) {
        *out_count = 0;
        return NULL;
    }
    size_t byte_array_size = (limit - 1) / 2;
    uint8_t *is_composite = (uint8_t *) calloc(byte_array_size, sizeof(uint8_t));
    if (!is_composite)
        return NULL;
    size_t prime_count = 1;                      
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
    uint32_t *primes = (uint32_t *) malloc(prime_count * sizeof(uint32_t));
    primes[0] = 2;
    size_t write_idx = 1;
    for (size_t i = 0; i < byte_array_size; i++) {
        if (!is_composite[i]) {
            primes[write_idx++] = (uint32_t) (2 * i + 3);
        }
    }
    free(is_composite);
    *out_count = prime_count;
    return primes;
}

extern void process_residue1(uint8_t * restrict segment, uint32_t size, SievingPrime * restrict primes, uint32_t count);
extern void process_residue7(uint8_t * restrict segment, uint32_t size, SievingPrime * restrict primes, uint32_t count);
extern void process_residue11(uint8_t * restrict segment, uint32_t size, SievingPrime * restrict primes, uint32_t count);
extern void process_residue13(uint8_t * restrict segment, uint32_t size, SievingPrime * restrict primes, uint32_t count);
extern void process_residue17(uint8_t * restrict segment, uint32_t size, SievingPrime * restrict primes, uint32_t count);
extern void process_residue19(uint8_t * restrict segment, uint32_t size, SievingPrime * restrict primes, uint32_t count);
extern void process_residue23(uint8_t * restrict segment, uint32_t size, SievingPrime * restrict primes, uint32_t count);
extern void process_residue29(uint8_t * restrict segment, uint32_t size, SievingPrime * restrict primes, uint32_t count);

const uint8_t mask_table[8][8] = {
    {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F},   
    {0xFD, 0xDF, 0xEF, 0xFE, 0x7F, 0xF7, 0xFB, 0xBF},   
    {0xFB, 0xEF, 0xFE, 0xBF, 0xFD, 0x7F, 0xF7, 0xDF},   
    {0xF7, 0xFE, 0xBF, 0xDF, 0xFB, 0xFD, 0x7F, 0xEF},   
    {0xEF, 0x7F, 0xFD, 0xFB, 0xDF, 0xBF, 0xFE, 0xF7},   
    {0xDF, 0xF7, 0x7F, 0xFD, 0xBF, 0xFE, 0xEF, 0xFB},   
    {0xBF, 0xFB, 0xF7, 0x7F, 0xFE, 0xEF, 0xDF, 0xFD},   
    {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE},   
};

const uint8_t offset_table[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 1},                    
    {1, 1, 1, 0, 1, 1, 1, 1},                    
    {2, 2, 0, 2, 0, 2, 2, 1},                    
    {3, 1, 1, 2, 1, 1, 3, 1},                    
    {3, 3, 1, 2, 1, 3, 3, 1},                    
    {4, 2, 2, 2, 2, 2, 4, 1},                    
    {5, 3, 1, 4, 1, 3, 5, 1},                    
    {6, 4, 2, 4, 2, 4, 6, 1},                    
};

const uint8_t bit_idx_map[30] = {
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 3, 0, 0, 0, 4, 0, 5, 0, 0, 0, 6, 0, 0, 0, 0, 0, 7
};

int main(int argc, char **argv) {
    uint64_t limit = 10000000000ULL;
    if (argc > 1) {
        limit = strtoull(argv[1], NULL, 10);
    }

    setlocale(LC_ALL, "");

    size_t l1_cache_size = get_min_cache_size(1);
    uint64_t segment_bytes = 1;
    while (segment_bytes * 2 <= l1_cache_size) {
        segment_bytes *= 2;
    }

    int num_threads = omp_get_max_threads();
    printf("Sieving to: %'lu\nSegment size: %'lu\nThreads: %u\n", limit, segment_bytes, num_threads);

    double start_time = omp_get_wtime();

    uint32_t seg_shift = __builtin_ctz(segment_bytes);
    uint32_t seg_mask = segment_bytes - 1;
    uint64_t numbers_per_segment = segment_bytes * 30;
    uint64_t total_segments = (limit / numbers_per_segment) + 1;

    size_t base_prime_count = 0;
    uint32_t *base_primes = generate_base_primes(limit, &base_prime_count);
    uint64_t total_primes = (limit >= 5) ? 3 : 0;
    // A prime must jump less than the segment to guarantee multiple hits
    uint32_t small_prime_threshold = segment_bytes * 3;
    uint64_t segments_per_thread = (total_segments + num_threads - 1) / num_threads;

    // --- PRE-SIEVE GENERATION ---
    // Generate the perfect 323KB repeating pattern for 7, 11, 13, 17, 19
    uint32_t pattern_size = 7 * 11 * 13 * 17 * 19; 
    uint8_t* pre_sieve_buffer = (uint8_t*)malloc(pattern_size + segment_bytes);
    memset(pre_sieve_buffer, 0xFF, pattern_size + segment_bytes);
    
    uint32_t pre_primes[] = {7, 11, 13, 17, 19};
    for (int i = 0; i < 5; i++) {
        uint32_t p = pre_primes[i];
        SievingPrime sp;
        sp.prime_k = p / 30;
        sp.prime_bit_idx = bit_idx_map[p % 30];
        sp.byte_index = p / 30; // First valid multiple is p * 1
        sp.wheel_index = 0;     // Multiplier 1 is at wheel index 0
        
        // Use the native unrolled functions (unrolled_xoff.c) to build the pattern
        if (p == 7)  process_residue7 (pre_sieve_buffer, pattern_size, &sp, 1);
        if (p == 11) process_residue11(pre_sieve_buffer, pattern_size, &sp, 1);
        if (p == 13) process_residue13(pre_sieve_buffer, pattern_size, &sp, 1);
        if (p == 17) process_residue17(pre_sieve_buffer, pattern_size, &sp, 1);
        if (p == 19) process_residue19(pre_sieve_buffer, pattern_size, &sp, 1);
    }
    
    // Copy the start of the pattern to the end to allow for seamless offset memcpys
    memcpy(pre_sieve_buffer + pattern_size, pre_sieve_buffer, segment_bytes);
    // -------------------------------

#pragma omp parallel reduction(+:total_primes)
    {
        int thread_id = omp_get_thread_num();
        uint64_t start_seg = thread_id * segments_per_thread;
        uint64_t end_seg = start_seg + segments_per_thread;
        if (end_seg > total_segments)
            end_seg = total_segments;

        if (start_seg < total_segments) {
            uint64_t thread_segment_count = end_seg - start_seg;
            SieveSegment *seg = create_segment(segment_bytes);

            SievingPrime *small_primes[8];
            uint32_t small_counts[8] = { 0 };
            for (int i = 0; i < 8; i++)
                small_primes[i] = (SievingPrime *) malloc(base_prime_count * sizeof(SievingPrime));

            // Circular buffer of 128 pointers (1KB, fits perfectly in L1)
            BucketNode *medium_buckets_heads[128] = { NULL };
            BucketPool *pool = create_bucket_pool(1000);
            if (!pool) {
                fprintf(stderr, "Thread %d: Failed to allocate bucket pool.\n", thread_id);
                exit(EXIT_FAILURE);
            }

            size_t next_base_prime_idx = 0;
            // --- SKIP PRE-SIEVED PRIMES ---
            // Skip 2, 3, 5, 7, 11, 13, 17, 19
            while (next_base_prime_idx < base_prime_count && base_primes[next_base_prime_idx] <= 19) {
                next_base_prime_idx++;
            }

            for (uint64_t seg_idx = start_seg; seg_idx < end_seg; seg_idx++) {
                uint64_t absolute_seg_start = seg_idx * numbers_per_segment;
                uint64_t absolute_seg_end = absolute_seg_start + numbers_per_segment;
                uint64_t local_seg_idx = seg_idx - start_seg;

                while (next_base_prime_idx < base_prime_count) {
                    uint32_t p = base_primes[next_base_prime_idx];
                    uint64_t p_squared = (uint64_t) p * p;

                    if (p_squared >= absolute_seg_end)
                        break;

                    uint8_t k_residue = 0;
                    uint64_t first_multiple = calculate_first_valid_multiple(p, absolute_seg_start, &k_residue);
                    uint64_t offset = first_multiple - absolute_seg_start;

                    SievingPrime sp;
                    sp.prime_k = p / 30;
                    sp.prime_bit_idx = bit_idx_map[p % 30];
                    sp.wheel_index = bit_idx_map[k_residue];

                    if (p <= small_prime_threshold) {
                        sp.byte_index = offset / 30;
                        small_primes[sp.prime_bit_idx][small_counts[sp.prime_bit_idx]++] = sp;
                    } else {
                        uint64_t relative_start = first_multiple - (start_seg * numbers_per_segment);
                        uint64_t total_bytes = relative_start / 30;
                        uint64_t target_local_seg = total_bytes >> seg_shift;
                        if (target_local_seg < thread_segment_count) {
                            sp.byte_index = total_bytes & seg_mask;
                            push_to_bucket(&medium_buckets_heads[target_local_seg & 127], sp, pool);
                        }
                    }
                    next_base_prime_idx++;
                }

                // --- MEMCPY HEAD START ---
                // Replace marking everything prime (0xFF) with the pre-calculated pattern
                uint32_t offset = (absolute_seg_start / 30) % pattern_size;
                memcpy(seg->array, pre_sieve_buffer + offset, segment_bytes);

                // Because our pattern physically crossed off 7, 11, 13, 17, 19, 
                // we must manually flip them back to "prime" status in the first segment
                if (seg_idx == 0) {
                    seg->array[0] &= 0xFE; // 1 is not a prime
                    seg->array[0] |= (1 << bit_idx_map[7]); 
                    seg->array[0] |= (1 << bit_idx_map[11]);
                    seg->array[0] |= (1 << bit_idx_map[13]);
                    seg->array[0] |= (1 << bit_idx_map[17]);
                    seg->array[0] |= (1 << bit_idx_map[19]);
                }
                // ----------------------------

                // SMALL primes 
                process_residue1(seg->array, segment_bytes, small_primes[0], small_counts[0]);
                process_residue7(seg->array, segment_bytes, small_primes[1], small_counts[1]);
                process_residue11(seg->array, segment_bytes, small_primes[2], small_counts[2]);
                process_residue13(seg->array, segment_bytes, small_primes[3], small_counts[3]);
                process_residue17(seg->array, segment_bytes, small_primes[4], small_counts[4]);
                process_residue19(seg->array, segment_bytes, small_primes[5], small_counts[5]);
                process_residue23(seg->array, segment_bytes, small_primes[6], small_counts[6]);
                process_residue29(seg->array, segment_bytes, small_primes[7], small_counts[7]);
                // ----------------------------

                // MEDIUM primes
                BucketNode *current_node = medium_buckets_heads[local_seg_idx & 127];
                while (current_node != NULL) {
                    if (current_node->next != NULL) {
                        __builtin_prefetch(current_node->next, 0, 1); 
                    }
                    for (uint32_t i = 0; i < current_node->count; i++) {
                        SievingPrime sp = current_node->primes[i];

                        // Process the prime as long as it stays in the current segment
                        do {
                            seg->array[sp.byte_index] &= mask_table[sp.prime_bit_idx][sp.wheel_index];
                            
                            uint32_t jump = (sp.prime_k * wheel_gaps[sp.wheel_index]) + offset_table[sp.prime_bit_idx][sp.wheel_index];
                            sp.byte_index += jump;
                            sp.wheel_index = (sp.wheel_index + 1) & 7;
                        } while (sp.byte_index < segment_bytes); // TRUE if it double-hits!

                        uint32_t segments_jumped = sp.byte_index >> seg_shift;
                        uint64_t target_local_seg = local_seg_idx + segments_jumped;
                        
                        if (target_local_seg < thread_segment_count) {
                            sp.byte_index &= seg_mask;
                            push_to_bucket(&medium_buckets_heads[target_local_seg & 127], sp, pool);
                        }
                    }
                    BucketNode *next_node = current_node->next;
                    return_node_to_pool(pool, current_node);
                    current_node = next_node;
                }
                // Erase the pointer so it is clean for the next rotation
                medium_buckets_heads[local_seg_idx & 127] = NULL;
                // ----------------------------

                // Fix up the final segment (so we're not counting beyond our limit)
                if (seg_idx == total_segments - 1) {
                    mask_last_segment(seg->array, segment_bytes, limit, absolute_seg_start);
                }

                // Count 'em!
                total_primes += count_primes_fast(seg->array, segment_bytes);
            }

            for (int i = 0; i < 8; i++) free(small_primes[i]);
            free_bucket_pool(pool);
            free_segment(seg);
        }
    }

    double end_time = omp_get_wtime();
    printf("Primes found: %'lu\n", total_primes);
    printf("Time elapsed: %f seconds\n", end_time - start_time);

    free(base_primes);
    free(pre_sieve_buffer);
    return 0;
}
