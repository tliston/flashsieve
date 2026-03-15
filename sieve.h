#ifndef SIEVE_H
#define SIEVE_H

#include <stdint.h>
#include <stddef.h>
#include "wheel.h"

#define BUCKET_CAPACITY 1024

// Define the signature for our unrolled functions
typedef void (*CrossOffFunc)(uint8_t *restrict, uint32_t, SievingPrime *restrict);

// Declare the array so main.c can see it
extern const CrossOffFunc cross_off_funcs[8];

typedef struct {
    uint8_t* array;
    size_t size;
} SieveSegment;

#define BUCKET_CAPACITY 1024

// We recycle your perfectly aligned 16-byte SievingPrime struct!
// 1024 * 16 bytes = 16KB per node + overhead (Fits beautifully in L1/L2 cache)
typedef struct BucketNode {
    SievingPrime primes[BUCKET_CAPACITY];
    uint32_t count;
    struct BucketNode* next;
} BucketNode;

// A single segment's bucket is just a pointer to a linked list of nodes
typedef struct {
    BucketNode* head;
} BucketList;

typedef struct {
    BucketNode* memory;
    BucketNode* free_list; // NEW: The top of our recycling stack
    uint32_t total_nodes;
    uint32_t current_node;
} BucketPool;

static inline BucketNode* get_node_from_pool(BucketPool* pool) {
    if (pool->free_list != NULL) {
        BucketNode* recycled_node = pool->free_list;
        pool->free_list = pool->free_list->next;
        recycled_node->next = NULL;
        return recycled_node;
    }
    return &pool->memory[pool->current_node++];
}

static inline void push_to_bucket(BucketList* list, SievingPrime sp, BucketPool* pool) {
    if (list->head == NULL || list->head->count == BUCKET_CAPACITY) {
        BucketNode* new_node = get_node_from_pool(pool);
        new_node->next = list->head;
        list->head = new_node;
    }
    BucketNode* head = list->head;
    head->primes[head->count++] = sp;
}

void return_node_to_pool(BucketPool* pool, BucketNode* node);
SieveSegment* create_segment(size_t cache_size);
void free_segment(SieveSegment* seg);
BucketPool* create_bucket_pool(uint32_t num_nodes);
void mask_last_segment(uint8_t* segment_array, uint32_t segment_bytes, uint64_t limit, uint64_t segment_start_val);
uint64_t count_primes_fast(const uint8_t* segment_array, uint32_t segment_bytes);
void free_bucket_pool(BucketPool* pool);
#endif // SIEVE_H
