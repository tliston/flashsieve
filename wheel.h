#ifndef WHEEL_H
#define WHEEL_H

#include <stdint.h>

// A single precomputed jump instruction
typedef struct {
    uint8_t next_bit_idx; 
    uint8_t byte_offset;  
} WheelItem;

// The state of a prime as it moves through the sieve
typedef struct {
    uint32_t prime_k;
    uint8_t  prime_bit_idx;
    uint32_t byte_index;
    uint8_t  bit_index;
    uint8_t  wheel_index;
} SievingPrime;

extern const WheelItem wheel_table[8][8];
extern const uint8_t wheel_gaps[8];
extern const uint8_t unset_bit[8];
extern const uint8_t valid_bits_mask[30];

uint8_t map_remainder_to_bit_idx(uint32_t remainder);
uint64_t calculate_first_valid_multiple(uint32_t p, uint64_t chunk_start_val, uint8_t* out_k_residue);

#endif // WHEEL_H
