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
 * \param[in] key Encryption key used to generate subkeys.
 * \param[in] plaintext Data to be encrypted.
 * \param[in] byte_count Number of bytes in ciphertext.
 * \param[out] ciphertext Encrypted data.
 * \return Number of encrypted bytes.
 */
size_t des_encrypt(uint64_t key, const char* plaintext, size_t byte_count,
    uint8_t* ciphertext);

/**
 * Decrypted ciphertext into a plaintext string using DES.
 *
 * \param[in] key Encryption key used to generate subkeys.
 * \param[in] ciphertext_count Number of bytes in ciphertext.
 * \param[in] ciphertext Data to be decrypted.
 * \param[in] plaintext_count Number of bytes in plaintext.
 * \param[out] plaintext Decrypted data.
 */
void des_decrypt(uint64_t key, size_t ciphertext_count, uint8_t* ciphertext,
    size_t plaintext_count, char* plaintext);

#endif
