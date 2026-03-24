#include "sieve.h"

// We still need the raw gaps to calculate our jumps
const uint8_t wheel_gaps[8] = { 6, 4, 2, 4, 2, 4, 6, 2 };

// For the sake of understanding how these values are created, they are: 
// ~(1<<0), ~(1<<1), ~(1<<2), ~(1<<3), ~(1<<4), ~(1<<5), ~(1<<6), ~(1<<7)
// We initialize them as uint8_t integers, to avoid a compiler warning...
const uint8_t unset_bit[8] = {
    254, 253, 251, 247, 239, 223, 191, 127
};

const uint8_t valid_bits_mask[30] = {
    0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x03, 0x03,
    0x03, 0x07, 0x07, 0x0F, 0x0F, 0x0F, 0x0F, 0x1F, 0x1F, 0x3F,
    0x3F, 0x3F, 0x3F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0xFF
};

uint64_t calculate_first_valid_multiple(uint32_t p, uint64_t chunk_start_val, uint8_t *out_k_residue) {
    if (chunk_start_val == 0)
        chunk_start_val = (uint64_t) p *p;
    uint64_t k = (chunk_start_val + p - 1) / p;
    if (k < p)
        k = p;
    while (k % 2 == 0 || k % 3 == 0 || k % 5 == 0) {
        k++;
    }
    *out_k_residue = (uint8_t) (k % 30);
    return k * p;
}

BucketPool *create_bucket_pool(uint32_t num_nodes) {
    // malloc used, so we must set pool->free_list to NULL
    // malloc used, so we must set current_node to 0
    BucketPool *pool = (BucketPool *) malloc(sizeof(BucketPool));
    if (!pool)
        return NULL;
    pool->free_list = NULL;
    pool->current_node = 0;
    pool->total_nodes = num_nodes;
    pool->memory = (BucketNode *) malloc(num_nodes * sizeof(BucketNode));
    if (!pool->memory) {
        free(pool);
        return NULL;
    }
    return pool;
}

void free_bucket_pool(BucketPool *pool) {
    if (pool) {
        free(pool->memory);
        free(pool);
    }
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
