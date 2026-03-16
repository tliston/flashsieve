#include "sieve.h"

/**
 * Pattern 0: For primes where MOD30(p) == 1
 */
void process_residue1(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict primes, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        SievingPrime *restrict sp = &primes[i];
        uint32_t p_k = sp->prime_k;
        uint32_t byte_idx = sp->byte_index;

        uint32_t j0 = p_k * 6 + 0;
        uint32_t j1 = p_k * 4 + 0;
        uint32_t j2 = p_k * 2 + 0;
        uint32_t j3 = p_k * 4 + 0;
        uint32_t j4 = p_k * 2 + 0;
        uint32_t j5 = p_k * 4 + 0;
        uint32_t j6 = p_k * 6 + 0;
        uint32_t j7 = p_k * 2 + 1;

        uint32_t p = (p_k * 30) + 1;
        uint32_t safe_limit = (size >= p) ? size - p : 0;

        if (byte_idx < safe_limit) {
            switch (sp->wheel_index) {
                case 0:
                    do {
                        segment[byte_idx] &= 0xFE; byte_idx += j0;
                // fall through
                case 1: segment[byte_idx] &= 0xFD; byte_idx += j1;
                // fall through
                case 2: segment[byte_idx] &= 0xFB; byte_idx += j2;
                // fall through
                case 3: segment[byte_idx] &= 0xF7; byte_idx += j3;
                // fall through
                case 4: segment[byte_idx] &= 0xEF; byte_idx += j4;
                // fall through
                case 5: segment[byte_idx] &= 0xDF; byte_idx += j5;
                // fall through
                case 6: segment[byte_idx] &= 0xBF; byte_idx += j6;
                // fall through
                case 7: segment[byte_idx] &= 0x7F; byte_idx += j7;
                    } while (byte_idx < safe_limit);
            }
            sp->wheel_index = 0;
        }

        switch (sp->wheel_index) {
            case 0:
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFE; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFD; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFB; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xF7; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xEF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xDF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xBF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0x7F; byte_idx += j7; sp->wheel_index = 0;
end_sieve_0:
                break;
            case 1:
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFD; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFB; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xF7; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xEF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xDF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xBF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0x7F; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFE; byte_idx += j0; sp->wheel_index = 1;
end_sieve_1:
                break;
            case 2:
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFB; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xF7; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xEF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xDF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xBF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0x7F; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFE; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFD; byte_idx += j1; sp->wheel_index = 2;
end_sieve_2:
                break;
            case 3:
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xF7; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xEF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xDF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xBF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0x7F; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFE; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFD; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFB; byte_idx += j2; sp->wheel_index = 3;
end_sieve_3:
                break;
            case 4:
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xEF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xDF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xBF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0x7F; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFE; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFD; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFB; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xF7; byte_idx += j3; sp->wheel_index = 4;
end_sieve_4:
                break;
            case 5:
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xDF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xBF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0x7F; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFE; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFD; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFB; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xF7; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xEF; byte_idx += j4; sp->wheel_index = 5;
end_sieve_5:
                break;
            case 6:
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xBF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0x7F; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFE; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFD; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFB; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xF7; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xEF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xDF; byte_idx += j5; sp->wheel_index = 6;
end_sieve_6:
                break;
            case 7:
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0x7F; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFE; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFD; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFB; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xF7; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xEF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xDF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xBF; byte_idx += j6; sp->wheel_index = 7;
end_sieve_7:
                break;
        }

        sp->byte_index = byte_idx - size;
    }
}

/**
 * Pattern 1: For primes where MOD30(p) == 7
 */
void process_residue7(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict primes, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        SievingPrime *restrict sp = &primes[i];
        uint32_t p_k = sp->prime_k;
        uint32_t byte_idx = sp->byte_index;

        uint32_t j0 = p_k * 6 + 1;
        uint32_t j1 = p_k * 4 + 1;
        uint32_t j2 = p_k * 2 + 1;
        uint32_t j3 = p_k * 4 + 0;
        uint32_t j4 = p_k * 2 + 1;
        uint32_t j5 = p_k * 4 + 1;
        uint32_t j6 = p_k * 6 + 1;
        uint32_t j7 = p_k * 2 + 1;

        uint32_t p = (p_k * 30) + 7;
        uint32_t safe_limit = (size >= p) ? size - p : 0;

        if (byte_idx < safe_limit) {
            switch (sp->wheel_index) {
                case 0:
                    do {
                        segment[byte_idx] &= 0xFD; byte_idx += j0;
                // fall through
                case 1: segment[byte_idx] &= 0xDF; byte_idx += j1;
                // fall through
                case 2: segment[byte_idx] &= 0xEF; byte_idx += j2;
                // fall through
                case 3: segment[byte_idx] &= 0xFE; byte_idx += j3;
                // fall through
                case 4: segment[byte_idx] &= 0x7F; byte_idx += j4;
                // fall through
                case 5: segment[byte_idx] &= 0xF7; byte_idx += j5;
                // fall through
                case 6: segment[byte_idx] &= 0xFB; byte_idx += j6;
                // fall through
                case 7: segment[byte_idx] &= 0xBF; byte_idx += j7;
                    } while (byte_idx < safe_limit);
            }
            sp->wheel_index = 0;
        }

        switch (sp->wheel_index) {
            case 0:
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFD; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xDF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xEF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFE; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0x7F; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xF7; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFB; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xBF; byte_idx += j7; sp->wheel_index = 0;
end_sieve_0:
                break;
            case 1:
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xDF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xEF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFE; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0x7F; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xF7; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFB; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xBF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFD; byte_idx += j0; sp->wheel_index = 1;
end_sieve_1:
                break;
            case 2:
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xEF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFE; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0x7F; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xF7; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFB; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xBF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFD; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xDF; byte_idx += j1; sp->wheel_index = 2;
end_sieve_2:
                break;
            case 3:
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFE; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0x7F; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xF7; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFB; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xBF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFD; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xDF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xEF; byte_idx += j2; sp->wheel_index = 3;
end_sieve_3:
                break;
            case 4:
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0x7F; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xF7; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFB; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xBF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFD; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xDF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xEF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFE; byte_idx += j3; sp->wheel_index = 4;
end_sieve_4:
                break;
            case 5:
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xF7; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFB; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xBF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFD; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xDF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xEF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFE; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0x7F; byte_idx += j4; sp->wheel_index = 5;
end_sieve_5:
                break;
            case 6:
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFB; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xBF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFD; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xDF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xEF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFE; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0x7F; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xF7; byte_idx += j5; sp->wheel_index = 6;
end_sieve_6:
                break;
            case 7:
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xBF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFD; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xDF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xEF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFE; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0x7F; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xF7; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFB; byte_idx += j6; sp->wheel_index = 7;
end_sieve_7:
                break;
        }

        sp->byte_index = byte_idx - size;
    }
}

/**
 * Pattern 2: For primes where MOD30(p) == 11
 */
void process_residue11(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict primes, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        SievingPrime *restrict sp = &primes[i];
        uint32_t p_k = sp->prime_k;
        uint32_t byte_idx = sp->byte_index;

        uint32_t j0 = p_k * 6 + 2;
        uint32_t j1 = p_k * 4 + 2;
        uint32_t j2 = p_k * 2 + 0;
        uint32_t j3 = p_k * 4 + 2;
        uint32_t j4 = p_k * 2 + 0;
        uint32_t j5 = p_k * 4 + 2;
        uint32_t j6 = p_k * 6 + 2;
        uint32_t j7 = p_k * 2 + 1;

        uint32_t p = (p_k * 30) + 11;
        uint32_t safe_limit = (size >= p) ? size - p : 0;

        if (byte_idx < safe_limit) {
            switch (sp->wheel_index) {
                case 0:
                    do {
                        segment[byte_idx] &= 0xFB; byte_idx += j0;
                // fall through
                case 1: segment[byte_idx] &= 0xEF; byte_idx += j1;
                // fall through
                case 2: segment[byte_idx] &= 0xFE; byte_idx += j2;
                // fall through
                case 3: segment[byte_idx] &= 0xBF; byte_idx += j3;
                // fall through
                case 4: segment[byte_idx] &= 0xFD; byte_idx += j4;
                // fall through
                case 5: segment[byte_idx] &= 0x7F; byte_idx += j5;
                // fall through
                case 6: segment[byte_idx] &= 0xF7; byte_idx += j6;
                // fall through
                case 7: segment[byte_idx] &= 0xDF; byte_idx += j7;
                    } while (byte_idx < safe_limit);
            }
            sp->wheel_index = 0;
        }

        switch (sp->wheel_index) {
            case 0:
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFB; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xEF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFE; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xBF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFD; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0x7F; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xF7; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xDF; byte_idx += j7; sp->wheel_index = 0;
end_sieve_0:
                break;
            case 1:
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xEF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFE; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xBF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFD; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0x7F; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xF7; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xDF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFB; byte_idx += j0; sp->wheel_index = 1;
end_sieve_1:
                break;
            case 2:
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFE; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xBF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFD; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0x7F; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xF7; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xDF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFB; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xEF; byte_idx += j1; sp->wheel_index = 2;
end_sieve_2:
                break;
            case 3:
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xBF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFD; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0x7F; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xF7; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xDF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFB; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xEF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFE; byte_idx += j2; sp->wheel_index = 3;
end_sieve_3:
                break;
            case 4:
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFD; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0x7F; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xF7; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xDF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFB; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xEF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFE; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xBF; byte_idx += j3; sp->wheel_index = 4;
end_sieve_4:
                break;
            case 5:
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0x7F; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xF7; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xDF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFB; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xEF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFE; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xBF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFD; byte_idx += j4; sp->wheel_index = 5;
end_sieve_5:
                break;
            case 6:
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xF7; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xDF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFB; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xEF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFE; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xBF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFD; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0x7F; byte_idx += j5; sp->wheel_index = 6;
end_sieve_6:
                break;
            case 7:
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xDF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFB; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xEF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFE; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xBF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFD; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0x7F; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xF7; byte_idx += j6; sp->wheel_index = 7;
end_sieve_7:
                break;
        }

        sp->byte_index = byte_idx - size;
    }
}

/**
 * Pattern 3: For primes where MOD30(p) == 13
 */
void process_residue13(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict primes, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        SievingPrime *restrict sp = &primes[i];
        uint32_t p_k = sp->prime_k;
        uint32_t byte_idx = sp->byte_index;

        uint32_t j0 = p_k * 6 + 3;
        uint32_t j1 = p_k * 4 + 1;
        uint32_t j2 = p_k * 2 + 1;
        uint32_t j3 = p_k * 4 + 2;
        uint32_t j4 = p_k * 2 + 1;
        uint32_t j5 = p_k * 4 + 1;
        uint32_t j6 = p_k * 6 + 3;
        uint32_t j7 = p_k * 2 + 1;

        uint32_t p = (p_k * 30) + 13;
        uint32_t safe_limit = (size >= p) ? size - p : 0;

        if (byte_idx < safe_limit) {
            switch (sp->wheel_index) {
                case 0:
                    do {
                        segment[byte_idx] &= 0xF7; byte_idx += j0;
                // fall through
                case 1: segment[byte_idx] &= 0xFE; byte_idx += j1;
                // fall through
                case 2: segment[byte_idx] &= 0xBF; byte_idx += j2;
                // fall through
                case 3: segment[byte_idx] &= 0xDF; byte_idx += j3;
                // fall through
                case 4: segment[byte_idx] &= 0xFB; byte_idx += j4;
                // fall through
                case 5: segment[byte_idx] &= 0xFD; byte_idx += j5;
                // fall through
                case 6: segment[byte_idx] &= 0x7F; byte_idx += j6;
                // fall through
                case 7: segment[byte_idx] &= 0xEF; byte_idx += j7;
                    } while (byte_idx < safe_limit);
            }
            sp->wheel_index = 0;
        }

        switch (sp->wheel_index) {
            case 0:
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xF7; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFE; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xBF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xDF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFB; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFD; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0x7F; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xEF; byte_idx += j7; sp->wheel_index = 0;
end_sieve_0:
                break;
            case 1:
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFE; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xBF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xDF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFB; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFD; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0x7F; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xEF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xF7; byte_idx += j0; sp->wheel_index = 1;
end_sieve_1:
                break;
            case 2:
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xBF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xDF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFB; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFD; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0x7F; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xEF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xF7; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFE; byte_idx += j1; sp->wheel_index = 2;
end_sieve_2:
                break;
            case 3:
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xDF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFB; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFD; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0x7F; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xEF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xF7; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFE; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xBF; byte_idx += j2; sp->wheel_index = 3;
end_sieve_3:
                break;
            case 4:
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFB; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFD; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0x7F; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xEF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xF7; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFE; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xBF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xDF; byte_idx += j3; sp->wheel_index = 4;
end_sieve_4:
                break;
            case 5:
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFD; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0x7F; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xEF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xF7; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFE; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xBF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xDF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFB; byte_idx += j4; sp->wheel_index = 5;
end_sieve_5:
                break;
            case 6:
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0x7F; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xEF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xF7; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFE; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xBF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xDF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFB; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFD; byte_idx += j5; sp->wheel_index = 6;
end_sieve_6:
                break;
            case 7:
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xEF; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xF7; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFE; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xBF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xDF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFB; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFD; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0x7F; byte_idx += j6; sp->wheel_index = 7;
end_sieve_7:
                break;
        }

        sp->byte_index = byte_idx - size;
    }
}

/**
 * Pattern 4: For primes where MOD30(p) == 17
 */
void process_residue17(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict primes, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        SievingPrime *restrict sp = &primes[i];
        uint32_t p_k = sp->prime_k;
        uint32_t byte_idx = sp->byte_index;

        uint32_t j0 = p_k * 6 + 3;
        uint32_t j1 = p_k * 4 + 3;
        uint32_t j2 = p_k * 2 + 1;
        uint32_t j3 = p_k * 4 + 2;
        uint32_t j4 = p_k * 2 + 1;
        uint32_t j5 = p_k * 4 + 3;
        uint32_t j6 = p_k * 6 + 3;
        uint32_t j7 = p_k * 2 + 1;

        uint32_t p = (p_k * 30) + 17;
        uint32_t safe_limit = (size >= p) ? size - p : 0;

        if (byte_idx < safe_limit) {
            switch (sp->wheel_index) {
                case 0:
                    do {
                        segment[byte_idx] &= 0xEF; byte_idx += j0;
                // fall through
                case 1: segment[byte_idx] &= 0x7F; byte_idx += j1;
                // fall through
                case 2: segment[byte_idx] &= 0xFD; byte_idx += j2;
                // fall through
                case 3: segment[byte_idx] &= 0xFB; byte_idx += j3;
                // fall through
                case 4: segment[byte_idx] &= 0xDF; byte_idx += j4;
                // fall through
                case 5: segment[byte_idx] &= 0xBF; byte_idx += j5;
                // fall through
                case 6: segment[byte_idx] &= 0xFE; byte_idx += j6;
                // fall through
                case 7: segment[byte_idx] &= 0xF7; byte_idx += j7;
                    } while (byte_idx < safe_limit);
            }
            sp->wheel_index = 0;
        }

        switch (sp->wheel_index) {
            case 0:
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xEF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0x7F; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFD; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFB; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xDF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xBF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFE; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xF7; byte_idx += j7; sp->wheel_index = 0;
end_sieve_0:
                break;
            case 1:
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0x7F; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFD; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFB; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xDF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xBF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFE; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xF7; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xEF; byte_idx += j0; sp->wheel_index = 1;
end_sieve_1:
                break;
            case 2:
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFD; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFB; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xDF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xBF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFE; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xF7; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xEF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0x7F; byte_idx += j1; sp->wheel_index = 2;
end_sieve_2:
                break;
            case 3:
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFB; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xDF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xBF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFE; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xF7; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xEF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0x7F; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFD; byte_idx += j2; sp->wheel_index = 3;
end_sieve_3:
                break;
            case 4:
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xDF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xBF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFE; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xF7; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xEF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0x7F; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFD; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFB; byte_idx += j3; sp->wheel_index = 4;
end_sieve_4:
                break;
            case 5:
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xBF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFE; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xF7; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xEF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0x7F; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFD; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFB; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xDF; byte_idx += j4; sp->wheel_index = 5;
end_sieve_5:
                break;
            case 6:
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFE; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xF7; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xEF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0x7F; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFD; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFB; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xDF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xBF; byte_idx += j5; sp->wheel_index = 6;
end_sieve_6:
                break;
            case 7:
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xF7; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xEF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0x7F; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFD; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFB; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xDF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xBF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFE; byte_idx += j6; sp->wheel_index = 7;
end_sieve_7:
                break;
        }

        sp->byte_index = byte_idx - size;
    }
}

/**
 * Pattern 5: For primes where MOD30(p) == 19
 */
void process_residue19(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict primes, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        SievingPrime *restrict sp = &primes[i];
        uint32_t p_k = sp->prime_k;
        uint32_t byte_idx = sp->byte_index;

        uint32_t j0 = p_k * 6 + 4;
        uint32_t j1 = p_k * 4 + 2;
        uint32_t j2 = p_k * 2 + 2;
        uint32_t j3 = p_k * 4 + 2;
        uint32_t j4 = p_k * 2 + 2;
        uint32_t j5 = p_k * 4 + 2;
        uint32_t j6 = p_k * 6 + 4;
        uint32_t j7 = p_k * 2 + 1;

        uint32_t p = (p_k * 30) + 19;
        uint32_t safe_limit = (size >= p) ? size - p : 0;

        if (byte_idx < safe_limit) {
            switch (sp->wheel_index) {
                case 0:
                    do {
                        segment[byte_idx] &= 0xDF; byte_idx += j0;
                // fall through
                case 1: segment[byte_idx] &= 0xF7; byte_idx += j1;
                // fall through
                case 2: segment[byte_idx] &= 0x7F; byte_idx += j2;
                // fall through
                case 3: segment[byte_idx] &= 0xFD; byte_idx += j3;
                // fall through
                case 4: segment[byte_idx] &= 0xBF; byte_idx += j4;
                // fall through
                case 5: segment[byte_idx] &= 0xFE; byte_idx += j5;
                // fall through
                case 6: segment[byte_idx] &= 0xEF; byte_idx += j6;
                // fall through
                case 7: segment[byte_idx] &= 0xFB; byte_idx += j7;
                    } while (byte_idx < safe_limit);
            }
            sp->wheel_index = 0;
        }

        switch (sp->wheel_index) {
            case 0:
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xDF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xF7; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0x7F; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFD; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xBF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFE; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xEF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFB; byte_idx += j7; sp->wheel_index = 0;
end_sieve_0:
                break;
            case 1:
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xF7; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0x7F; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFD; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xBF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFE; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xEF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFB; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xDF; byte_idx += j0; sp->wheel_index = 1;
end_sieve_1:
                break;
            case 2:
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0x7F; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFD; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xBF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFE; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xEF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFB; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xDF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xF7; byte_idx += j1; sp->wheel_index = 2;
end_sieve_2:
                break;
            case 3:
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFD; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xBF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFE; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xEF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFB; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xDF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xF7; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0x7F; byte_idx += j2; sp->wheel_index = 3;
end_sieve_3:
                break;
            case 4:
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xBF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFE; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xEF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFB; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xDF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xF7; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0x7F; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFD; byte_idx += j3; sp->wheel_index = 4;
end_sieve_4:
                break;
            case 5:
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFE; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xEF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFB; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xDF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xF7; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0x7F; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFD; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xBF; byte_idx += j4; sp->wheel_index = 5;
end_sieve_5:
                break;
            case 6:
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xEF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFB; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xDF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xF7; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0x7F; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFD; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xBF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFE; byte_idx += j5; sp->wheel_index = 6;
end_sieve_6:
                break;
            case 7:
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFB; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xDF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xF7; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0x7F; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFD; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xBF; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFE; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xEF; byte_idx += j6; sp->wheel_index = 7;
end_sieve_7:
                break;
        }

        sp->byte_index = byte_idx - size;
    }
}

/**
 * Pattern 6: For primes where MOD30(p) == 23
 */
void process_residue23(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict primes, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        SievingPrime *restrict sp = &primes[i];
        uint32_t p_k = sp->prime_k;
        uint32_t byte_idx = sp->byte_index;

        uint32_t j0 = p_k * 6 + 5;
        uint32_t j1 = p_k * 4 + 3;
        uint32_t j2 = p_k * 2 + 1;
        uint32_t j3 = p_k * 4 + 4;
        uint32_t j4 = p_k * 2 + 1;
        uint32_t j5 = p_k * 4 + 3;
        uint32_t j6 = p_k * 6 + 5;
        uint32_t j7 = p_k * 2 + 1;

        uint32_t p = (p_k * 30) + 23;
        uint32_t safe_limit = (size >= p) ? size - p : 0;

        if (byte_idx < safe_limit) {
            switch (sp->wheel_index) {
                case 0:
                    do {
                        segment[byte_idx] &= 0xBF; byte_idx += j0;
                // fall through
                case 1: segment[byte_idx] &= 0xFB; byte_idx += j1;
                // fall through
                case 2: segment[byte_idx] &= 0xF7; byte_idx += j2;
                // fall through
                case 3: segment[byte_idx] &= 0x7F; byte_idx += j3;
                // fall through
                case 4: segment[byte_idx] &= 0xFE; byte_idx += j4;
                // fall through
                case 5: segment[byte_idx] &= 0xEF; byte_idx += j5;
                // fall through
                case 6: segment[byte_idx] &= 0xDF; byte_idx += j6;
                // fall through
                case 7: segment[byte_idx] &= 0xFD; byte_idx += j7;
                    } while (byte_idx < safe_limit);
            }
            sp->wheel_index = 0;
        }

        switch (sp->wheel_index) {
            case 0:
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xBF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFB; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xF7; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0x7F; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFE; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xEF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xDF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFD; byte_idx += j7; sp->wheel_index = 0;
end_sieve_0:
                break;
            case 1:
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFB; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xF7; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0x7F; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFE; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xEF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xDF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFD; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xBF; byte_idx += j0; sp->wheel_index = 1;
end_sieve_1:
                break;
            case 2:
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xF7; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0x7F; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFE; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xEF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xDF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFD; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xBF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFB; byte_idx += j1; sp->wheel_index = 2;
end_sieve_2:
                break;
            case 3:
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0x7F; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFE; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xEF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xDF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFD; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xBF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFB; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xF7; byte_idx += j2; sp->wheel_index = 3;
end_sieve_3:
                break;
            case 4:
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFE; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xEF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xDF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFD; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xBF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFB; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xF7; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0x7F; byte_idx += j3; sp->wheel_index = 4;
end_sieve_4:
                break;
            case 5:
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xEF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xDF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFD; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xBF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFB; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xF7; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0x7F; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFE; byte_idx += j4; sp->wheel_index = 5;
end_sieve_5:
                break;
            case 6:
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xDF; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFD; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xBF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFB; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xF7; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0x7F; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFE; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xEF; byte_idx += j5; sp->wheel_index = 6;
end_sieve_6:
                break;
            case 7:
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFD; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xBF; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFB; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xF7; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0x7F; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFE; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xEF; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xDF; byte_idx += j6; sp->wheel_index = 7;
end_sieve_7:
                break;
        }

        sp->byte_index = byte_idx - size;
    }
}

/**
 * Pattern 7: For primes where MOD30(p) == 29
 */
void process_residue29(uint8_t *restrict segment, uint32_t size, SievingPrime *restrict primes, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        SievingPrime *restrict sp = &primes[i];
        uint32_t p_k = sp->prime_k;
        uint32_t byte_idx = sp->byte_index;

        uint32_t j0 = p_k * 6 + 6;
        uint32_t j1 = p_k * 4 + 4;
        uint32_t j2 = p_k * 2 + 2;
        uint32_t j3 = p_k * 4 + 4;
        uint32_t j4 = p_k * 2 + 2;
        uint32_t j5 = p_k * 4 + 4;
        uint32_t j6 = p_k * 6 + 6;
        uint32_t j7 = p_k * 2 + 1;

        uint32_t p = (p_k * 30) + 29;
        uint32_t safe_limit = (size >= p) ? size - p : 0;

        if (byte_idx < safe_limit) {
            switch (sp->wheel_index) {
                case 0:
                    do {
                        segment[byte_idx] &= 0x7F; byte_idx += j0;
                // fall through
                case 1: segment[byte_idx] &= 0xBF; byte_idx += j1;
                // fall through
                case 2: segment[byte_idx] &= 0xDF; byte_idx += j2;
                // fall through
                case 3: segment[byte_idx] &= 0xEF; byte_idx += j3;
                // fall through
                case 4: segment[byte_idx] &= 0xF7; byte_idx += j4;
                // fall through
                case 5: segment[byte_idx] &= 0xFB; byte_idx += j5;
                // fall through
                case 6: segment[byte_idx] &= 0xFD; byte_idx += j6;
                // fall through
                case 7: segment[byte_idx] &= 0xFE; byte_idx += j7;
                    } while (byte_idx < safe_limit);
            }
            sp->wheel_index = 0;
        }

        switch (sp->wheel_index) {
            case 0:
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0x7F; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xBF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xDF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xEF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xF7; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFB; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFD; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_0;
                segment[byte_idx] &= 0xFE; byte_idx += j7; sp->wheel_index = 0;
end_sieve_0:
                break;
            case 1:
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xBF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xDF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xEF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xF7; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFB; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFD; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0xFE; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_1;
                segment[byte_idx] &= 0x7F; byte_idx += j0; sp->wheel_index = 1;
end_sieve_1:
                break;
            case 2:
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xDF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xEF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xF7; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFB; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFD; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xFE; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0x7F; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_2;
                segment[byte_idx] &= 0xBF; byte_idx += j1; sp->wheel_index = 2;
end_sieve_2:
                break;
            case 3:
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xEF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xF7; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFB; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFD; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xFE; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0x7F; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xBF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_3;
                segment[byte_idx] &= 0xDF; byte_idx += j2; sp->wheel_index = 3;
end_sieve_3:
                break;
            case 4:
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xF7; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFB; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFD; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xFE; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0x7F; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xBF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xDF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_4;
                segment[byte_idx] &= 0xEF; byte_idx += j3; sp->wheel_index = 4;
end_sieve_4:
                break;
            case 5:
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFB; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFD; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xFE; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0x7F; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xBF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xDF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xEF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_5;
                segment[byte_idx] &= 0xF7; byte_idx += j4; sp->wheel_index = 5;
end_sieve_5:
                break;
            case 6:
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFD; byte_idx += j6; sp->wheel_index = 7;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFE; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0x7F; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xBF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xDF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xEF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xF7; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_6;
                segment[byte_idx] &= 0xFB; byte_idx += j5; sp->wheel_index = 6;
end_sieve_6:
                break;
            case 7:
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFE; byte_idx += j7; sp->wheel_index = 0;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0x7F; byte_idx += j0; sp->wheel_index = 1;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xBF; byte_idx += j1; sp->wheel_index = 2;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xDF; byte_idx += j2; sp->wheel_index = 3;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xEF; byte_idx += j3; sp->wheel_index = 4;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xF7; byte_idx += j4; sp->wheel_index = 5;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFB; byte_idx += j5; sp->wheel_index = 6;
                if (byte_idx >= size) goto end_sieve_7;
                segment[byte_idx] &= 0xFD; byte_idx += j6; sp->wheel_index = 7;
end_sieve_7:
                break;
        }

        sp->byte_index = byte_idx - size;
    }
}

