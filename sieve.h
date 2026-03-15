#ifndef SIEVE_H
#define SIEVE_H

#include <stdint.h>
#include <stddef.h>
#include "wheel.h"

#define BUCKET_CAPACITY 1024

typedef struct {
    uint8_t *array;
    size_t size;
} SieveSegment;

// We recycle the perfectly aligned 16-byte SievingPrime struct!
// 1024 * 16 bytes = 16KB per node + overhead (Fits beautifully in L1/L2 cache)
typedef struct BucketNode {
    SievingPrime primes[BUCKET_CAPACITY];
    uint32_t count;
    struct BucketNode *next;
} BucketNode;

typedef struct {
    BucketNode *memory;
    BucketNode *free_list;
    uint32_t total_nodes;
    uint32_t current_node;
} BucketPool;

static inline BucketNode *get_node_from_pool(BucketPool *pool) {
    if (pool->free_list != NULL) {
        BucketNode *recycled_node = pool->free_list;
        pool->free_list = pool->free_list->next;
        recycled_node->next = NULL;
        return recycled_node;
    }
    return &pool->memory[pool->current_node++];
}

static inline void push_to_bucket(BucketNode *head, SievingPrime sp, BucketPool *pool) {
    if (head == NULL || head->count == BUCKET_CAPACITY) {
        BucketNode *new_node = get_node_from_pool(pool);
        new_node->next = head;
        head = new_node;
    }
    BucketNode *new_head = head;
    new_head->primes[new_head->count++] = sp;
}

// prototypes

BucketPool *create_bucket_pool(uint32_t num_nodes);
void return_node_to_pool(BucketPool * pool, BucketNode * node);
void free_bucket_pool(BucketPool * pool);

SieveSegment *create_segment(size_t cache_size);
void free_segment(SieveSegment * seg);
void mask_last_segment(uint8_t * segment_array, uint32_t segment_bytes, uint64_t limit, uint64_t segment_start_val);

uint64_t count_primes_fast(const uint8_t * segment_array, uint32_t segment_bytes);

#endif // SIEVE_H
