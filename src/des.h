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
 * Generates all sixteen 48-bit subkeys needed for encryption.
 *
 * \param[in] key 64-bit original key.
 * \param[out] subkeys All sixteen 48-bit subkeys, stored as 64-bit values.
 */
void generate_subkeys(uint64_t key, uint64_t subkeys[16]);

#endif
