#ifndef WHEEL_H
#define WHEEL_H

#include <stdint.h>

// A single precomputed jump instruction
typedef struct {
    uint8_t next_bit_idx; 
    uint8_t byte_offset;  
} WheelItem;

typedef struct {
    uint32_t prime_k;       // 4 bytes - WE BROUGHT IT BACK!
    uint32_t byte_index;    // 4 bytes
    uint8_t  prime_bit_idx; // 1 byte (0-7, tells us which pattern to run)
    uint8_t  wheel_index;   // 1 byte (0-7, tells Duff's device where to jump in)
    uint8_t  pad[6];        // 6 bytes padding to lock it perfectly to 16 bytes
} SievingPrime;

extern const WheelItem wheel_table[8][8];
extern const uint8_t wheel_gaps[8];
extern const uint8_t unset_bit[8];
extern const uint8_t valid_bits_mask[30];

uint8_t map_remainder_to_bit_idx(uint32_t remainder);
uint64_t calculate_first_valid_multiple(uint32_t p, uint64_t chunk_start_val, uint8_t* out_k_residue);

#endif // WHEEL_H
