/**
 * \file des.h
 * \author Isaiah Lateer
 *
 * Interface for DES.
 */

#ifndef DES_HEADER
#define DES_HEADER

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/**
 * Encrypts a file using DES.
 * 
 * \param[in] key Encryption key used to generate subkeys.
 * \param[in] plaintext File to be encrypted.
 * \param[in] byte_count Number of bytes in ciphertext.
 * \param[out] ciphertext Encrypted data.
 * \return Number of encrypted bytes.
 */
size_t des_encrypt(uint64_t key, FILE* plaintext, size_t byte_count,
    uint8_t* ciphertext);

/**
 * Decrypts a file using DES.
 *
 * \param[in] key Encryption key used to generate subkeys.
 * \param[in] ciphertext File to be decrypted.
 * \param[in] byte_count Number of bytes in plaintext.
 * \param[out] plaintext Decrypted data.
 * \return Number of decrypted bytes.
 */
size_t des_decrypt(uint64_t key, FILE* ciphertext, size_t byte_count,
    uint8_t* plaintext);

#endif
