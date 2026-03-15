#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sieve.h"
#include "wheel.h"

// Memory masks for clearing the exact bit in the Medium buckets
const uint8_t mask_table[8][8] = {
    { 0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F }, // Prime residue 1
    { 0xFD, 0xDF, 0xEF, 0xFE, 0x7F, 0xF7, 0xFB, 0xBF }, // Prime residue 7
    { 0xFB, 0xEF, 0xFE, 0xBF, 0xFD, 0x7F, 0xF7, 0xDF }, // Prime residue 11
    { 0xF7, 0xFE, 0xBF, 0xDF, 0xFB, 0xFD, 0x7F, 0xEF }, // Prime residue 13
    { 0xEF, 0x7F, 0xFD, 0xFB, 0xDF, 0xBF, 0xFE, 0xF7 }, // Prime residue 17
    { 0xDF, 0xF7, 0x7F, 0xFD, 0xBF, 0xFE, 0xEF, 0xFB }, // Prime residue 19
    { 0xBF, 0xFB, 0xF7, 0x7F, 0xFE, 0xEF, 0xDF, 0xFD }, // Prime residue 23
    { 0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE }, // Prime residue 29
};

// Byte offsets for the next jump calculation (Carry-over math)
const uint8_t offset_table[8][8] = {
    { 0, 0, 0, 0, 0, 0, 0, 1 }, // Prime residue 1
    { 1, 1, 1, 0, 1, 1, 1, 1 }, // Prime residue 7
    { 2, 2, 0, 2, 0, 2, 2, 1 }, // Prime residue 11
    { 3, 1, 1, 2, 1, 1, 3, 1 }, // Prime residue 13
    { 3, 3, 1, 2, 1, 3, 3, 1 }, // Prime residue 17
    { 4, 2, 2, 2, 2, 2, 4, 1 }, // Prime residue 19
    { 5, 3, 1, 4, 1, 3, 5, 1 }, // Prime residue 23
    { 6, 4, 2, 4, 2, 4, 6, 1 }, // Prime residue 29
};

void process_medium_bucket(uint8_t* restrict segment, BucketList* list, BucketList* next_segment_buckets, uint64_t segment_bytes, BucketPool *pool) {
    BucketNode* current_node = list->head;
    
    // Iterate through the linked list of bucket nodes
    while (current_node != NULL) {
        // Process the 1024 primes inside this node in a tight, branchless loop
        for (uint32_t i = 0; i < current_node->count; i++) {
            SievingPrime sp = current_node->primes[i];            
            // 1. One-shot cross-off (Branchless!)
            segment[sp.byte_index] &= mask_table[sp.prime_bit_idx][sp.wheel_index];            
            // 2. Calculate the next jump (Branchless!)
            uint32_t jump = (sp.prime_k * wheel_gaps[sp.wheel_index]) + offset_table[sp.prime_bit_idx][sp.wheel_index];            
            // 3. Update state
            sp.byte_index += jump;
            sp.wheel_index = (sp.wheel_index + 1) & 7; // Fast modulo 8            
            // 4. Calculate which future segment this lands in
            uint32_t future_segment = sp.byte_index / segment_bytes;
            sp.byte_index %= segment_bytes; // Localize byte index to that future segment
            // 5. Push to the correct future bucket
            push_to_bucket(&next_segment_buckets[future_segment], sp, pool);
        }
        current_node = current_node->next;
    }
}

BucketPool* create_bucket_pool(uint32_t num_nodes) {
    BucketPool* pool = (BucketPool*)malloc(sizeof(BucketPool));
    if (!pool) return NULL;
    
    // Allocate all nodes in one massive, contiguous block of RAM
    pool->memory = (BucketNode*)calloc(num_nodes, sizeof(BucketNode));
    if (!pool->memory) {
        free(pool);
        return NULL;
    }
    pool->total_nodes = num_nodes;
    pool->current_node = 0;
    return pool;
}

void free_bucket_pool(BucketPool* pool) {
    if (pool) {
        free(pool->memory);
        free(pool);
    }
}

// O(1) recycling - Wipe the node and push it to the top of the free list
inline void return_node_to_pool(BucketPool* pool, BucketNode* node) {
    node->count = 0; // Wipe it clean for its next life
    node->next = pool->free_list;
    pool->free_list = node;
}

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
