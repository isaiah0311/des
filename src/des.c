/**
 * \file des.c
 * \author Isaiah Lateer
 *
 * Implementation of the DES interface.
 */

#include "des.h"

#include <stdio.h>
#include <string.h>

static const int pc1[] = { 57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31,
    23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28,
    20, 12, 4 };

static const int shifts[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

static const int pc2[] = { 14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19,
    12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45,
    33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 };

static const int ip[] = { 58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20,
    12, 4, 62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8, 57, 49,
    41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45, 37, 29, 21,
    13, 5, 63, 55, 47, 39, 31, 23, 15, 7 };

static const int e[] = { 32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12,
    13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1 };

static const uint8_t s1[] = { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0,
    7, 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, 4, 1, 14, 8, 13, 6,
    2, 11, 15, 12, 9, 7, 3, 10, 5, 0, 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14,
    10, 0, 6, 13 };

static const uint8_t s2[] = { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5,
    10, 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5, 0, 14, 7, 11, 10,
    4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15, 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12,
    0, 5, 14, 9 };

static const uint8_t s3[] = { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2,
    8, 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1, 13, 6, 4, 9, 8, 15,
    3, 0, 11, 1, 2, 12, 5, 10, 14, 7, 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3,
    11, 5, 2, 12 };

static const uint8_t s4[] = { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4,
    15, 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9, 10, 6, 9, 0, 12,
    11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4, 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11,
    12, 7, 2, 14 };

static const uint8_t s5[] = { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14,
    9, 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6, 4, 2, 1, 11, 10,
    13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14, 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0,
    9, 10, 4, 5, 3 };

static const uint8_t s6[] = { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5,
    11, 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8, 9, 14, 15, 5, 2,
    8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6, 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1,
    7, 6, 0, 8, 13 };

static const uint8_t s7[] = { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6,
    1, 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6, 1, 4, 11, 13, 12,
    3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2, 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15,
    14, 2, 3, 12 };

static const uint8_t s8[] = { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12,
    7, 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2, 7, 11, 4, 1, 9, 12,
    14, 2, 0, 6, 10, 13, 15, 3, 5, 8, 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0,
    3, 5, 6, 11 };

static const int p[] = { 16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18,
    31, 10, 2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25 };

static const int fp[] = { 40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23,
    63, 31, 38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 36, 4,
    44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50,
    18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25 };

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
static void des_generate_subkeys(uint64_t key, uint64_t subkeys[16]) {
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

/**
 * Constructs a 64-bit block from 8 bytes.
 *
 * \param[in] bytes Bytes to construct with.
 * \return 64-bit block.
 */
uint64_t des_construct_block(const uint8_t bytes[8]) {
    uint64_t block = 0;
    for (int j = 0; j < 8; ++j) {
        block <<= 8;
        block |= bytes[j];
    }

    return block;
}

/**
 * Rearranges the bits of a block according to the initial permutation
 * table. Each bit from the original block is extracted and placed into its
 * new position to produce the permuted block.
 *
 * \param[in] block Block to permutate.
 * \return Permuted block.
 */
static uint64_t des_initial_permutation(uint64_t block) {
    uint64_t permuted = 0;
    for (int i = 0; i < 64; ++i) {
        const int index = ip[i] - 1;
        const uint64_t bit = (block >> (63 - index)) & 1;
        permuted |= bit << (63 - i);
    }

    return permuted;
}

/**
 * Splits a 64-bit block into two 32-bit halves. The most significant 32
 * bits of the block are stored in the left half, and the least significant
 * 32 bits are stored in right.
 *
 * \param[in] block 64-bit block to split.
 * \param[out] left 32-bit left half.
 * \param[out] right 32-bit right half.
 */
static void des_split_block(uint64_t block, uint32_t* left, uint32_t* right) {
    *left = (block >> 32) & 0xFFFFFFFF;
    *right = block & 0xFFFFFFFF;
}

/**
 * Takes the 32-bit right half of a block and expands it to 48 bits by
 * duplicating and permuting bits according to the expansion table.
 *
 * \param[in] right 32-bit right half.
 * \return 48-bit expanded block.
 */
static uint64_t des_expand(uint32_t right) {
    uint64_t expanded = 0;
    for (int i = 0; i < 48; ++i) {
        const uint64_t bit = (right >> (32 - e[i])) & 1;
        expanded |= bit << (47 - i);
    }

    return expanded;
}

/**
 * Performs non-linear substitution on a block. Divides the 48-bit block
 * into eight 6-bit chunks. Each 6-bit chunk is mapped to a 4-bit output
 * using one of the eight s-boxes. The first and last bits of the 6-bit
 * chunk select the row. The middle four bits select the column. The eight
 * 4-bit outputs are concatenated to form a single 32-bit substituted block.
 *
 * \param[in] mixed 48-bit XOR'd expanded right half with the round subkey.
 * \return 32-bit substitued block.
 */
static uint32_t des_substitute(uint64_t mixed) {
    uint32_t substituted = 0;
    for (uint8_t i = 0; i < 8; ++i) {
        const uint8_t bits = (mixed >> (42 - 6 * i)) & 0x3F;
        const uint8_t row = ((bits & 0x20) >> 4) | (bits & 0x01);
        const uint8_t col = (bits >> 1) & 0x0F;

        uint8_t value = 0;
        switch (i) {
        case 0:
            value = s1[row * 16 + col];
            break;
        case 1:
            value = s2[row * 16 + col];
            break;
        case 2:
            value = s3[row * 16 + col];
            break;
        case 3:
            value = s4[row * 16 + col];
            break;
        case 4:
            value = s5[row * 16 + col];
            break;
        case 5:
            value = s6[row * 16 + col];
            break;
        case 6:
            value = s7[row * 16 + col];
            break;
        case 7:
            value = s8[row * 16 + col];
            break;
        }

        substituted |= (value << (28 - 4 * i));
    }

    return substituted;
}

/**
 * Permutes the 32-bit block by rearranging its bits according to the
 * permutation table.
 *
 * \param[in] substituted Post s-box substitution block.
 * \return Permuted block.
 */
static uint32_t des_permutate(uint32_t substituted) {
    uint32_t permuted = 0;
    for (int i = 0; i < 32; ++i) {
        const uint32_t bit = (substituted >> (32 - p[i])) & 1;
        permuted |= bit << (31 - i);
    }

    return permuted;
}

/**
 * Performs the core transformation applied to the right half of the block
 * in each round.
 *
 * \param[in] right Right half.
 * \param[in] subkey 48-bit round subkey stored as a 64-bit value.
 * \return Post round right half.
 */
static uint32_t des_feistel(uint32_t right, uint64_t subkey) {
    const uint64_t expanded = des_expand(right);
    const uint64_t mixed = expanded ^ subkey;
    const uint32_t substituted = des_substitute(mixed);
    return des_permutate(substituted);
}

/**
 * Performs a single round on both halves of a block.
 *
 * \param[in,out] left Left half.
 * \param[in,out] right Right half.
 * \param[in] subkey 48-bit round subkey stored as a 64-bit value.
 */
static void des_round(uint32_t* left, uint32_t* right, uint64_t subkey) {
    const uint32_t temp = *right;
    *right = *left ^ des_feistel(*right, subkey);
    *left = temp;
}

/**
 * Performs the inverse of the initial permutation. It rearranges the bits
 * of the block according to the final permutation table, producing the
 * final 64-bit output block.
 *
 * \param[in] block Block to be permuted.
 * \return Permuted block.
 */
static uint64_t des_final_permutation(uint64_t block) {
    uint64_t permuted = 0;
    for (int i = 0; i < 64; ++i) {
        const int index = fp[i] - 1;
        const uint64_t bit = (block >> (63 - index)) & 1;
        permuted |= bit << (63 - i);
    }

    return permuted;
}

/**
 * Takes a 64-bit block and applies an initial permutation. The permuted block
 * is than put through 16 feistel rounds and then a final permutation.
 *
 * \param[in] subkeys Sixteen 48-bit keys for each of the feistel rounds.
 * \param[in] block Data block.
 * \return Resulting block.
 */
uint64_t des(const uint64_t subkeys[16], uint64_t block) {
    const uint64_t permuted = des_initial_permutation(block);

    uint32_t left, right;
    des_split_block(permuted, &left, &right);

    for (int j = 0; j < 16; ++j) {
        des_round(&left, &right, subkeys[j]);
    }

    const uint64_t swapped = ((uint64_t) right << 32) | left;
    return des_final_permutation(swapped);
}

/**
 * Splits the plaintext into 64-bit blocks, padding the final block with zeroes.
 * Each of the blocks are encrypted using the key and placed into the ciphertext
 * buffer.
 *
 * \param[in] key Encryption key used to generate subkeys.
 * \param[in] plaintext Data to be encrypted.
 * \param[in] byte_count Number of bytes in ciphertext.
 * \param[out] ciphertext Encrypted data.
 * \return Number of encrypted bytes.
 */
size_t des_encrypt(uint64_t key, const char* plaintext, size_t byte_count,
    uint8_t* ciphertext) {
    memset(ciphertext, 0, byte_count);

    uint64_t subkeys[16] = { 0 };
    des_generate_subkeys(key, subkeys);

    const size_t char_count = strlen(plaintext);
    const size_t block_count =
        (byte_count < char_count ? (byte_count + 7) / 8 : (char_count + 7) / 8);

    for (size_t i = 0; i < block_count; ++i) {
        uint8_t bytes[8] = { 0 };
        const size_t copy_count =
            (i * 8 + 8 <= char_count) ? 8 : char_count - i * 8;
        memcpy(bytes, plaintext + i * 8, copy_count);

        const uint64_t block = des_construct_block(bytes);
        const uint64_t encrypted = des(subkeys, block);

        ciphertext[(i * 8)] = (uint8_t) ((encrypted >> 56) & 0xFF);
        ciphertext[(i * 8) + 1] = (uint8_t) ((encrypted >> 48) & 0xFF);
        ciphertext[(i * 8) + 2] = (uint8_t) ((encrypted >> 40) & 0xFF);
        ciphertext[(i * 8) + 3] = (uint8_t) ((encrypted >> 32) & 0xFF);
        ciphertext[(i * 8) + 4] = (uint8_t) ((encrypted >> 24) & 0xFF);
        ciphertext[(i * 8) + 5] = (uint8_t) ((encrypted >> 16) & 0xFF);
        ciphertext[(i * 8) + 6] = (uint8_t) ((encrypted >> 8) & 0xFF);
        ciphertext[(i * 8) + 7] = (uint8_t) (encrypted & 0xFF);
    }

    return block_count * 8;
}

/**
 * Splits the ciphertext into 64-bit blocks, padding the final block with
 * zeroes. Each of the blocks are decrypted using the key and placed into the
 * plaintext buffer.
 *
 * \param[in] key Encryption key used to generate subkeys.
 * \param[in] ciphertext_count Number of bytes in ciphertext.
 * \param[in] ciphertext Data to be decrypted.
 * \param[in] plaintext_count Number of bytes in plaintext.
 * \param[out] plaintext Decrypted data.
 */
void des_decrypt(uint64_t key, size_t ciphertext_count, uint8_t* ciphertext,
    size_t plaintext_count, char* plaintext) {
    memset(plaintext, 0, plaintext_count);

    uint64_t subkeys[16] = { 0 };
    des_generate_subkeys(key, subkeys);

    for (int i = 0; i < 8; ++i) {
        uint64_t subkey = subkeys[i];
        subkeys[i] = subkeys[15 - i];
        subkeys[15 - i] = subkey;
    }

    const size_t block_count =
        (ciphertext_count < plaintext_count ? (ciphertext_count + 7) / 8
                                            : (plaintext_count + 7) / 8);

    for (size_t i = 0; i < block_count; ++i) {
        uint8_t bytes[8] = { 0 };
        const size_t copy_count =
            (i * 8 + 8 <= ciphertext_count) ? 8 : ciphertext_count - i * 8;
        memcpy(bytes, ciphertext + i * 8, copy_count);

        const uint64_t block = des_construct_block(bytes);
        const uint64_t decrypted = des(subkeys, block);

        plaintext[(i * 8)] = (char) ((decrypted >> 56) & 0xFF);
        plaintext[(i * 8) + 1] = (char) ((decrypted >> 48) & 0xFF);
        plaintext[(i * 8) + 2] = (char) ((decrypted >> 40) & 0xFF);
        plaintext[(i * 8) + 3] = (char) ((decrypted >> 32) & 0xFF);
        plaintext[(i * 8) + 4] = (char) ((decrypted >> 24) & 0xFF);
        plaintext[(i * 8) + 5] = (char) ((decrypted >> 16) & 0xFF);
        plaintext[(i * 8) + 6] = (char) ((decrypted >> 8) & 0xFF);
        plaintext[(i * 8) + 7] = (char) (decrypted & 0xFF);
    }
}
