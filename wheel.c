#include "wheel.h"

// We still need the raw gaps to calculate the 'gk' part of the jump
const uint8_t wheel_gaps[8] = { 6, 4, 2, 4, 2, 4, 6, 2 };

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

uint64_t calculate_first_valid_multiple(uint32_t p, uint64_t chunk_start_val, uint8_t *out_k_residue) {
    if (chunk_start_val == 0)
        chunk_start_val = (uint64_t) p *p;
    uint64_t k = (chunk_start_val + p - 1) / p;
    if (k < p)
        k = p;
    while (k % 2 == 0 || k % 3 == 0 || k % 5 == 0) {
        k++;
    }
    *out_k_residue = (uint8_t) (k % 30);
    return k * p;
}
