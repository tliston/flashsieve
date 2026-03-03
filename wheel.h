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
    uint32_t byte_index;     // 4 bytes
    uint8_t  bit_index;      // 1 byte
    uint8_t  wheel_index;    // 1 byte
    uint8_t  pad[2];         // 2 bytes (Memory alignment padding)    
    // Precomputed jump arrays (no more math in the inner loop!)
    uint32_t jumps[8];       // 32 bytes
    uint8_t  next_bits[8];   // 8 bytes
} SievingPrime; 
// Total Size: 48 bytes (Fits completely in a 64-byte cache line)

extern const WheelItem wheel_table[8][8];
extern const uint8_t wheel_gaps[8];
extern const uint8_t unset_bit[8];
extern const uint8_t valid_bits_mask[30];

uint8_t map_remainder_to_bit_idx(uint32_t remainder);
uint64_t calculate_first_valid_multiple(uint32_t p, uint64_t chunk_start_val, uint8_t* out_k_residue);

#endif // WHEEL_H
