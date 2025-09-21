/**
 * \file des.h
 * \author Isaiah Lateer
 *
 * Interface for DES.
 */

#ifndef DES_HEADER
#define DES_HEADER

#include <stdint.h>

/**
 * Encrypts a plaintext string using DES.
 * 
 * \param[in] key Encryption key.
 * \param[in] plaintext Data to be encrypted.
 */
void des_encrypt(uint64_t key, const char* plaintext);

#endif
