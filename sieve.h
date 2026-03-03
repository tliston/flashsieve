#ifndef SIEVE_H
#define SIEVE_H

#include <stdint.h>
#include <stddef.h>
#include "wheel.h"

#define BUCKET_CAPACITY 1024

typedef struct {
    uint8_t* array;
    size_t size;
} SieveSegment;

typedef struct BucketNode {
    SievingPrime primes[BUCKET_CAPACITY];
    uint32_t count;
    struct BucketNode* next;
} BucketNode;

typedef struct {
    BucketNode* head;
} Bucket;

SieveSegment* create_segment(size_t cache_size);
void free_segment(SieveSegment* seg);

void cross_off_multiples_fast(uint8_t* segment_array, uint32_t segment_bytes, SievingPrime* sp);
void mask_last_segment(uint8_t* segment_array, uint32_t segment_bytes, uint64_t limit, uint64_t segment_start_val);
uint64_t count_primes_fast(const uint8_t* segment_array, uint32_t segment_bytes);

#endif // SIEVE_H
