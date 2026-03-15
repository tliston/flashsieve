#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sieve.h"
#include "wheel.h"

BucketPool *create_bucket_pool(uint32_t num_nodes) {
    BucketPool *pool = (BucketPool *) malloc(sizeof(BucketPool));
    if (!pool)
        return NULL;

    // Allocate all nodes in one massive, contiguous block of RAM
    pool->memory = (BucketNode *) calloc(num_nodes, sizeof(BucketNode));
    if (!pool->memory) {
        free(pool);
        return NULL;
    }
    pool->total_nodes = num_nodes;
    pool->current_node = 0;
    return pool;
}

void free_bucket_pool(BucketPool *pool) {
    if (pool) {
        free(pool->memory);
        free(pool);
    }
}

// O(1) recycling - Wipe the node and push it to the top of the free list
inline void return_node_to_pool(BucketPool *pool, BucketNode *node) {
    node->count = 0;                             // Wipe it clean for its next life
    node->next = pool->free_list;
    pool->free_list = node;
}

SieveSegment *create_segment(size_t cache_size) {
    SieveSegment *seg = (SieveSegment *) malloc(sizeof(SieveSegment));
    if (!seg)
        return NULL;
    seg->size = cache_size;
    seg->array = (uint8_t *) aligned_alloc(64, cache_size);
    if (!seg->array) {
        free(seg);
        return NULL;
    }
    return seg;
}

void free_segment(SieveSegment *seg) {
    if (seg) {
        free(seg->array);
        free(seg);
    }
}

void mask_last_segment(uint8_t *segment_array, uint32_t segment_bytes, uint64_t limit, uint64_t segment_start_val) {
    if (limit < segment_start_val) {
        memset(segment_array, 0, segment_bytes);
        return;
    }
    uint64_t limit_offset = limit - segment_start_val;
    uint64_t byte_idx = limit_offset / 30;
    uint32_t remainder = limit_offset % 30;

    if (byte_idx >= segment_bytes)
        return;

    segment_array[byte_idx] &= valid_bits_mask[remainder];
    if (byte_idx + 1 < segment_bytes) {
        memset(&segment_array[byte_idx + 1], 0, segment_bytes - (byte_idx + 1));
    }
}

uint64_t count_primes_fast(const uint8_t *segment_array, uint32_t segment_bytes) {
    const uint64_t *array_64 = (const uint64_t *)segment_array;
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
