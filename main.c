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

int main(int argc, char** argv) {
    uint64_t limit = 10000000000UL; // Default to 10 billion
    if (argc > 1)
        limit = strtoull(argv[1], NULL, 10);
    setlocale(LC_ALL, "");    
    printf("Sieving to: %'lu\n", limit);
    double start_time = omp_get_wtime();

    size_t l1_cache_size = get_min_cache_size(1);
    uint64_t segment_bytes = l1_cache_size;
    uint64_t numbers_per_segment = segment_bytes * 30; 
    uint64_t total_segments = (limit / numbers_per_segment) + 1;
    size_t base_prime_count = 0;
    uint32_t* base_primes = generate_base_primes(limit, &base_prime_count);

    // Count 2, 3, 5 manually since the Mod 30 wheel entirely skips them
    uint64_t total_primes = (limit >= 5) ? 2 : 0; 
    // Calculate the block size per thread to ensure contiguous state-machine execution
    int num_threads = omp_get_max_threads();
    printf("Number of threads: %i\n", num_threads);
    uint64_t segments_per_thread = (total_segments + num_threads - 1) / num_threads;
    // We use OpenMP to spawn the threads, but WE manually control their start/stop bounds
    #pragma omp parallel reduction(+:total_primes)
    {
        int thread_id = omp_get_thread_num();       
        uint64_t start_seg = thread_id * segments_per_thread;
        uint64_t end_seg = start_seg + segments_per_thread;
        if (end_seg > total_segments) {
            end_seg = total_segments;
        }        
        // Only spin up work if this thread actually has segments assigned to it
        if (start_seg < total_segments) {
            SieveSegment* seg = create_segment(segment_bytes);            
            // We still allocate enough space for all base primes, 
            // but we don't initialize them yet.
            SievingPrime* active_primes = (SievingPrime*)malloc(base_prime_count * sizeof(SievingPrime));            
            size_t active_prime_count = 0;
            size_t next_base_prime_idx = 0;
            // Skip 2, 3, and 5 just like before
            while (next_base_prime_idx < base_prime_count && base_primes[next_base_prime_idx] <= 5)
                next_base_prime_idx++;
            // --- THE DYNAMIC SIEVING ENGINE ---
            for (uint64_t seg_idx = start_seg; seg_idx < end_seg; seg_idx++) {                
                uint64_t absolute_seg_start = seg_idx * numbers_per_segment;
                uint64_t absolute_seg_end = absolute_seg_start + numbers_per_segment;
                // 1. ACTIVATE PENDING PRIMES
                // If the next prime's square falls within or before this segment, activate it!
                while (next_base_prime_idx < base_prime_count) {
                    uint32_t p = base_primes[next_base_prime_idx];
                    uint64_t p_squared = (uint64_t)p * p;                    
                    // If p^2 is beyond the end of this segment, stop activating.
                    if (p_squared >= absolute_seg_end)
                        break; 
                    // Do the heavy initialization math ONLY when the prime is needed
                    uint8_t k_residue = 0;
                    uint64_t first_multiple = calculate_first_valid_multiple(p, absolute_seg_start, &k_residue);
                    uint64_t offset = first_multiple - absolute_seg_start;

                    active_primes[active_prime_count].prime_k = p / 30;
                    active_primes[active_prime_count].byte_index = offset / 30;
                    active_primes[active_prime_count].prime_bit_idx = map_remainder_to_bit_idx(p % 30);
                    active_primes[active_prime_count].wheel_index = map_remainder_to_bit_idx(k_residue);

                    active_prime_count++;
                    next_base_prime_idx++;
                }
                // 2. THE INNER LOOP
                // Set all bits to 1
                memset(seg->array, 0xFF, segment_bytes); 
                // We now ONLY loop over primes that are mathematically active
                for (size_t i = 0; i < active_prime_count; i++) {
                    SievingPrime* sp = &active_primes[i];
                    // Call the specific unrolled function for this prime's pattern!
                    cross_off_funcs[sp->prime_bit_idx](seg->array, segment_bytes, sp);
                }
                // 3. MASK AND COUNT
                if (seg_idx == total_segments - 1) {
                    mask_last_segment(seg->array, segment_bytes, limit, absolute_seg_start);
                }
                total_primes += count_primes_fast(seg->array, segment_bytes);
            }
            free(active_primes);
            free_segment(seg);
        }
    }
    double end_time = omp_get_wtime();
    printf("Primes found: %'lu\n", total_primes);
    printf("Time elapsed: %f seconds\n", end_time - start_time);
    free(base_primes);
    return 0;
}
