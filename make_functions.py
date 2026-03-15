#!/usr/bin/env python

valid_residues = [1, 7, 11, 13, 17, 19, 23, 29]
wheel_gaps = [6, 4, 2, 4, 2, 4, 6, 2]

print('#include "wheel.h"\n')

for p_idx, p_rem in enumerate(valid_residues):
    print(f"/**\n * Pattern {p_idx}: For primes where MOD30(p) == {p_rem}\n */")
    print(f"void process_residue{p_rem}(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict primes, uint32_t count) {{")
    print("    for (uint32_t i = 0; i < count; i++) {")
    print("        SievingPrime *restrict sp = &primes[i];")
    print("        uint32_t p_k = sp->prime_k;")
    print("        uint32_t byte_idx = sp->byte_index;\n")
    
    masks = []
    for w_idx in range(8):
        k_rem = valid_residues[w_idx]
        gap = wheel_gaps[w_idx]
        current_rem = (p_rem * k_rem) % 30
        bit_idx = valid_residues.index(current_rem)
        mask = (~(1 << bit_idx)) & 0xFF
        masks.append(mask)
        byte_offset = (current_rem + (p_rem * gap)) // 30
        print(f"        uint32_t j{w_idx} = p_k * {gap} + {byte_offset};")
        
    print(f"\n        uint32_t p = (p_k * 30) + {p_rem};")
    print("        uint32_t safe_limit = (size >= p) ? size - p : 0;\n")
    
    print("        if (byte_idx < safe_limit) {")
    print("            switch (sp->wheel_index) {")
    print("                case 0:")
    print("                    do {")
    for w_idx in range(8):
        case_str = f"                case {w_idx}:" if w_idx > 0 else "                       "
        print(f"{case_str} segment[byte_idx] &= 0x{masks[w_idx]:02X}; byte_idx += j{w_idx};")
        if(w_idx < 7):
            print("                // fall through") 
    print("                    } while (byte_idx < safe_limit);")
    print("            }")
    print("            sp->wheel_index = 0;")
    print("        }\n")
    
    print("        switch (sp->wheel_index) {")
    for start_case in range(8):
        print(f"            case {start_case}:")
        for step in range(8):
            w_idx = (start_case + step) % 8
            next_w = (w_idx + 1) % 8
            print(f"                if (byte_idx >= size) goto end_sieve_{start_case};")
            print(f"                segment[byte_idx] &= 0x{masks[w_idx]:02X}; byte_idx += j{w_idx}; sp->wheel_index = {next_w};")
        print(f"end_sieve_{start_case}:")
        print("                break;")
    print("        }\n")
    print("        sp->byte_index = byte_idx - size;")
    print("    }")
    print("}\n")
