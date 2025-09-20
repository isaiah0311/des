/**
 * \file des.c
 * \author Isaiah Lateer
 *
 * Implementation of the DES interface.
 */

#include "des.h"

static const int pc1[] = { 57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31,
    23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28,
    20, 12, 4 };

static const int shifts[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

static const int pc2[] = { 14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19,
    12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45,
    33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 };

/**
 * Applies the permuted choice 1 table to the original 64-bit key and reduces it
 * to 56 bits. Splits the 56-bit permuted key into two 28-bit halves, C and D.
 * For each of the 16 rounds, left-rotates both halves according to the
 * round-specific shift value, combines the shifted halves back into a 56-bit
 * key, and then applies the permuted choice 2 table to produce a 48-bit
 * subkey.
 *
 * \param[in] key 64-bit original key.
 * \param[out] subkeys All sixteen 48-bit subkeys, stored as 64-bit values.
 */
void generate_subkeys(uint64_t key, uint64_t subkeys[16]) {
    uint64_t permuted = 0;
    for (int i = 0; i < 56; ++i) {
        permuted |= ((key >> (64 - pc1[i])) & 1) << (55 - i);
    }

    uint64_t c = permuted >> 28;
    uint64_t d = permuted & 0xFFFFFFF;

    for (int i = 0; i < 16; ++i) {
        const int shift = shifts[i];
        c = ((c << shift) | (c >> (28 - shift))) & 0xFFFFFFF;
        d = ((d << shift) | (d >> (28 - shift))) & 0xFFFFFFF;

        const uint64_t combined = (c << 28) | d;

        uint64_t subkey = 0;
        for (int j = 0; j < 48; ++j) {
            subkey |= ((combined >> (56 - pc2[j])) & 1) << (47 - j);
        }

        subkeys[i] = subkey;
    }
}
