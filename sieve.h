#ifndef SIEVE_H
#define SIEVE_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUCKET_CAPACITY 1024

// typedefs

typedef struct {
    uint8_t *array;
    size_t size;
} SieveSegment;

typedef struct {
    uint32_t prime_k;
    uint32_t p_k2;
    uint32_t p_k4;
    uint32_t p_k6;
    uint32_t byte_index;
    uint8_t prime_bit_idx;
    uint8_t wheel_index;
} SievingPrime;

// We use the perfectly aligned 16-byte SievingPrime struct
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

extern const uint8_t wheel_gaps[8];
extern const uint8_t unset_bit[8];
extern const uint8_t valid_bits_mask[30];

// these are kept here to allow them to be easily inlined into main.c

static inline BucketNode *get_node_from_pool(BucketPool *pool) {
    if (pool->free_list != NULL) {
        BucketNode *recycled_node = pool->free_list;
        pool->free_list = pool->free_list->next;
        recycled_node->next = NULL;
        return recycled_node;
    } 
    // The safety boundary!
    if (pool->current_node >= pool->total_nodes) {
        fprintf(stderr, "Fatal: Bucket pool exhausted! A memory leak occurred.\n");
        exit(EXIT_FAILURE);
    }
    return &pool->memory[pool->current_node++];
}

static inline void push_to_bucket(BucketNode **head_ptr, SievingPrime sp, BucketPool *pool) {
    // Dereference to see what the array slot currently points to
    BucketNode *head = *head_ptr; 
    
    if (head == NULL || head->count == BUCKET_CAPACITY) {
        BucketNode *new_node = get_node_from_pool(pool);
        new_node->next = head;
        
        // Update the actual array back in main.c!
        *head_ptr = new_node; 
        
        head = new_node;
    }
    
    head->primes[head->count++] = sp;
}

// O(1) recycling - Wipe the node and push it to the top of the free list
inline void return_node_to_pool(BucketPool *pool, BucketNode *node) {
    node->count = 0;                             // Wipe it clean for its next life
    node->next = pool->free_list;
    pool->free_list = node;
}

// prototypes

BucketPool *create_bucket_pool(uint32_t);
void free_bucket_pool(BucketPool*);

SieveSegment *create_segment(size_t);
void free_segment(SieveSegment*);
void mask_last_segment(uint8_t*, uint32_t, uint64_t, uint64_t);

uint64_t count_primes_fast(const uint8_t*, uint32_t);
uint64_t calculate_first_valid_multiple(uint32_t, uint64_t, uint8_t*);

#endif // SIEVE_H
