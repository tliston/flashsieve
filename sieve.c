#include "sieve.h"
#include <stdlib.h>
#include <string.h>

// Declare the 8 generated functions so this file knows they exist
extern void cross_off_residue1(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict sp);
extern void cross_off_residue7(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict sp);
extern void cross_off_residue11(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict sp);
extern void cross_off_residue13(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict sp);
extern void cross_off_residue17(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict sp);
extern void cross_off_residue19(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict sp);
extern void cross_off_residue23(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict sp);
extern void cross_off_residue29(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict sp);

// Map them to the 0-7 bit indexes
const CrossOffFunc cross_off_funcs[8] = {
    cross_off_residue1,  // Index 0
    cross_off_residue7,  // Index 1
    cross_off_residue11, // Index 2
    cross_off_residue13, // Index 3
    cross_off_residue17, // Index 4
    cross_off_residue19, // Index 5
    cross_off_residue23, // Index 6
    cross_off_residue29  // Index 7
};

// (You can completely delete the old cross_off_multiples_fast function from this file!)

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
