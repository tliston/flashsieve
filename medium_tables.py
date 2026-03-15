#!/usr/bin/env python

valid_residues = [1, 7, 11, 13, 17, 19, 23, 29]
wheel_gaps = [6, 4, 2, 4, 2, 4, 6, 2]

print("// Memory masks for clearing the exact bit in the Medium buckets")
print("const uint8_t mask_table[8][8] = {")
for p_rem in valid_residues:
    masks = []
    for w_idx in range(8):
        k_rem = valid_residues[w_idx]
        current_rem = (p_rem * k_rem) % 30
        bit_idx = valid_residues.index(current_rem)
        mask = (~(1 << bit_idx)) & 0xFF
        masks.append(f"0x{mask:02X}")
    print("    { " + ", ".join(masks) + " }, // Prime residue " + str(p_rem))
print("};\n")

print("// Byte offsets for the next jump calculation (Carry-over math)")
print("const uint8_t offset_table[8][8] = {")
for p_rem in valid_residues:
    offsets = []
    for w_idx in range(8):
        k_rem = valid_residues[w_idx]
        gap = wheel_gaps[w_idx]
        current_rem = (p_rem * k_rem) % 30
        
        # Calculate the exact byte jump offset
        byte_offset = (current_rem + (p_rem * gap)) // 30
        offsets.append(str(byte_offset))
    print("    { " + ", ".join(offsets) + " }, // Prime residue " + str(p_rem))
print("};")
