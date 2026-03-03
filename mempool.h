#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

typedef struct FreeBlock {
    struct FreeBlock *next;                      // Pointer to next free block
} FreeBlock;

typedef struct {
    void *memory;                                // Pre-allocated memory block
    size_t item_size;                            // Size of each item
    size_t capacity;                             // Total number of items the pool can hold
    size_t count;                                // Number of items currently in use
    FreeBlock *free_list;                        // Linked list of free blocks
    pthread_mutex_t lock;                        // Mutex for thread safety
} memorypool;

// function prototypes
memorypool *mempool_create(size_t item_size, size_t capacity);
void *mempool_alloc(memorypool * pool);
void mempool_free(memorypool * pool, void *ptr);
void mempool_destroy(memorypool * pool);

#endif // MEMPOOL_H
