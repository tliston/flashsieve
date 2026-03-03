#include "mempool.h"

/**
 * Create a new memory pool for a specific item size
 */
memorypool *mempool_create(size_t item_size, size_t capacity) {
    // Make sure item_size is at least as large as FreeBlock
    size_t actual_item_size = (item_size > sizeof(FreeBlock)) ? item_size : sizeof(FreeBlock);

    memorypool *pool = (memorypool *) malloc(sizeof(memorypool));
    if (!pool)
        return NULL;

    // Initialize pool properties
    pool->item_size = actual_item_size;
    pool->capacity = capacity;
    pool->count = 0;
    pool->free_list = NULL;

    // Initialize mutex
    if (pthread_mutex_init(&pool->lock, NULL) != 0) {
        free(pool);
        return NULL;
    }
    // Allocate memory for all potential items
    pool->memory = malloc(actual_item_size * capacity);
    if (!pool->memory) {
        pthread_mutex_destroy(&pool->lock);
        free(pool);
        return NULL;
    }
    // Initialize free list
    // Link each block to the next one
    char *block = (char *)pool->memory;
    for (size_t i = 0; i < capacity; i++) {
        FreeBlock *free_block = (FreeBlock *) block;
        free_block->next = pool->free_list;
        pool->free_list = free_block;
        block += actual_item_size;
    }

    return pool;
}

/**
 * Allocate an item from the pool or fallback to malloc if pool is full
 * O(1) operation - just takes the first free block
 */
void *mempool_alloc(memorypool *pool) {
    if (!pool)
        return NULL;

    void *result = NULL;

    // Lock the mutex before accessing shared resources
    pthread_mutex_lock(&pool->lock);

    if (pool->free_list) {
        // Get the first free block
        FreeBlock *block = pool->free_list;
        pool->free_list = block->next;
        result = (void *)block;
        pool->count++;
    }

    pthread_mutex_unlock(&pool->lock);

    // If no free blocks, fallback to malloc
    if (!result)
        return malloc(pool->item_size);

    return result;
}

/**
 * Free an item back to the pool or use standard free if not from pool
 * O(1) operation - just adds to the head of the free list
 */
void mempool_free(memorypool *pool, void *ptr) {
    if (!pool || !ptr)
        return;

    // Check if this pointer belongs to our pool
    if (!(ptr >= pool->memory && ptr < (void *)((char *)pool->memory + (pool->capacity * pool->item_size)))) {
        // Not from our pool, use standard free
        free(ptr);
        return;
    }
    // Lock the mutex before modifying the pool
    pthread_mutex_lock(&pool->lock);

    // Add the block to the free list
    FreeBlock *block = (FreeBlock *) ptr;
    block->next = pool->free_list;
    pool->free_list = block;
    pool->count--;

    pthread_mutex_unlock(&pool->lock);
}

/**
 * Destroy the memory pool
 */
void mempool_destroy(memorypool *pool) {
    if (!pool)
        return;

    pthread_mutex_lock(&pool->lock);
    free(pool->memory);
    pthread_mutex_unlock(&pool->lock);

    pthread_mutex_destroy(&pool->lock);
    free(pool);
}
