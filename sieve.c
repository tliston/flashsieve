#include "sieve.h"
#include <stdlib.h>
#include <string.h>

SieveSegment* create_segment(size_t cache_size) {
    SieveSegment* seg = (SieveSegment*)malloc(sizeof(SieveSegment));
    if (!seg) return NULL;
    seg->size = cache_size;
    seg->array = (uint8_t*)aligned_alloc(64, cache_size);
    if (!seg->array) {
        free(seg);
        return NULL;
    }
    return seg;
}

void free_segment(SieveSegment* seg) {
    if (seg) {
        free(seg->array); 
        free(seg);
    }
}

void cross_off_multiples_fast(uint8_t* segment_array, uint32_t segment_bytes, SievingPrime* sp) {
    uint32_t byte_idx  = sp->byte_index;
    uint8_t  bit_idx   = sp->bit_index;
    uint8_t  wheel_idx = sp->wheel_index;
    uint32_t p_k = sp->prime_k;
    const WheelItem* precomputed_row = wheel_table[sp->prime_bit_idx];

    while (byte_idx < segment_bytes) {
        segment_array[byte_idx] &= unset_bit[bit_idx];
        WheelItem jump_info = precomputed_row[wheel_idx];
        byte_idx += (wheel_gaps[wheel_idx] * p_k) + jump_info.byte_offset;
        bit_idx = jump_info.next_bit_idx;
        wheel_idx = (wheel_idx + 1) & 7; 
    }

    sp->byte_index  = byte_idx - segment_bytes;
    sp->bit_index   = bit_idx;
    sp->wheel_index = wheel_idx;
}

void mask_last_segment(uint8_t* segment_array, uint32_t segment_bytes, uint64_t limit, uint64_t segment_start_val) {
    if (limit < segment_start_val) {
        memset(segment_array, 0, segment_bytes);
        return;
    }
    uint64_t limit_offset = limit - segment_start_val;
    uint64_t byte_idx = limit_offset / 30;
    uint32_t remainder = limit_offset % 30;

    if (byte_idx >= segment_bytes) return;

    segment_array[byte_idx] &= valid_bits_mask[remainder];
    if (byte_idx + 1 < segment_bytes) {
        memset(&segment_array[byte_idx + 1], 0, segment_bytes - (byte_idx + 1));
    }
}

uint64_t count_primes_fast(const uint8_t* segment_array, uint32_t segment_bytes) {
    const uint64_t* array_64 = (const uint64_t*)segment_array;
    uint32_t count_64 = segment_bytes / sizeof(uint64_t);
    
    uint64_t count0 = 0, count1 = 0, count2 = 0, count3 = 0;

    for (size_t i = 0; i < count_64; i += 4) {
        count0 += __builtin_popcountll(array_64[i]);
        count1 += __builtin_popcountll(array_64[i + 1]);
        count2 += __builtin_popcountll(array_64[i + 2]);
        count3 += __builtin_popcountll(array_64[i + 3]);
    }
    return count0 + count1 + count2 + count3;
}
