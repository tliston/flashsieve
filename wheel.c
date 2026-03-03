#include "wheel.h"

// 2D Lookup Table: wheel_table[prime_bit_index][current_wheel_index]
// There are 8 types of primes, and 8 steps in the wheel.
const WheelItem wheel_table[8][8] = {
    // Prime residue 1 (bit_idx 0)
    // Gaps: 6, 4, 2, 4, 2, 4, 6, 2
    { {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {0, 0} },
    // Prime residue 7 (bit_idx 1)
    // Note how the byte offsets > 0 appear because 7 * gap can exceed 30
    { {3, 1}, {6, 0}, {7, 0}, {1, 1}, {2, 0}, {4, 1}, {5, 1}, {0, 0} },
    // Prime residue 11 (bit_idx 2)
    { {5, 2}, {7, 1}, {0, 1}, {3, 1}, {4, 0}, {6, 1}, {1, 2}, {2, 0} },
    // Prime residue 13 (bit_idx 3)
    { {6, 2}, {1, 2}, {3, 0}, {5, 1}, {7, 0}, {0, 2}, {2, 2}, {4, 0} },
    // Prime residue 17 (bit_idx 4)
    { {7, 3}, {4, 2}, {6, 1}, {0, 2}, {2, 1}, {3, 2}, {5, 3}, {1, 1} },
    // Prime residue 19 (bit_idx 5)
    { {0, 3}, {5, 2}, {1, 1}, {4, 2}, {6, 1}, {7, 2}, {3, 3}, {2, 1} },
    // Prime residue 23 (bit_idx 6)
    { {2, 4}, {3, 3}, {5, 1}, {7, 3}, {0, 2}, {1, 3}, {4, 4}, {6, 1} },
    // Prime residue 29 (bit_idx 7)
    { {4, 5}, {0, 4}, {2, 2}, {6, 4}, {1, 2}, {5, 4}, {3, 5}, {7, 2} }
};

// We still need the raw gaps to calculate the 'gk' part of the jump
const uint8_t wheel_gaps[8] = {6, 4, 2, 4, 2, 4, 6, 2};

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

const uint8_t bit_idx_map[30] = {
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 3, 0, 0, 0, 4, 0, 5, 0, 0, 0, 6, 0, 0, 0, 0, 0, 7
};

uint8_t map_remainder_to_bit_idx(uint32_t remainder) {
    return bit_idx_map[remainder];
}

uint64_t calculate_first_valid_multiple(uint32_t p, uint64_t chunk_start_val, uint8_t* out_k_residue) {
    if (chunk_start_val == 0) chunk_start_val = p * p;
    uint64_t k = (chunk_start_val + p - 1) / p;
    if (k < p) k = p;    
    while (k % 2 == 0 || k % 3 == 0 || k % 5 == 0) {
        k++;
    }
    *out_k_residue = (uint8_t)(k % 30); 
    return k * p;
}
