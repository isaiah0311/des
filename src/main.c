/**
 * \file main.c
 * \author Isaiah Lateer
 *
 * Entry point for the project.
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "des.h"

#define BUFFER_SIZE 1024

/**
 * Prompts the user for a key and then generates the subkeys.
 *
 * \return Exit code.
 */
int main() {
    printf("Enter key: ");
    char buffer[BUFFER_SIZE] = { 0 };
    char* result = fgets(buffer, sizeof(buffer) - 1, stdin);
    if (!result) {
        fprintf(stderr, "[ERROR] Failed to read key.\n");
        return EXIT_FAILURE;
    }

    buffer[strcspn(buffer, "\r\n")] = 0;

    char* endptr = NULL;
    uint64_t key = strtoull(buffer, &endptr, 10);
    if (endptr == buffer) {
        fprintf(stderr, "[ERROR] Key was not a number.\n");
        return EXIT_FAILURE;
    }

    uint64_t subkeys[16] = { 0 };
    generate_subkeys(key, subkeys);

    for (int i = 0; i < 16; ++i) {
        printf("Subkey %2d: 0x%012" PRIX64 "\n", i + 1, subkeys[i]);
    }

    return EXIT_SUCCESS;
}
