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

/**
 * Encrypts a plaintext string using DES.
 * 
 * \param[in] key Encryption key.
 * \param[in] plaintext Data to be encrypted.
 * \param[in] byte_count Number of bytes the ciphertext is.
 * \param[out] ciphertext Encrypted data.
 * \return Number of encrypted bytes.
 */
size_t des_encrypt(uint64_t key, const char* plaintext, size_t byte_count,
    uint8_t* ciphertext);

#endif
