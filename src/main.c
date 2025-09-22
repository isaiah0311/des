/**
 * \file main.c
 * \author Isaiah Lateer
 *
 * Entry point for the project.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "des.h"

#define BUFFER_SIZE 1024

/**
 * Prompts the user for a key and plaintext.
 *
 * \return Exit code.
 */
int main() {
    char buffer[BUFFER_SIZE] = { 0 };

    printf("Enter key: 0x");
    char* result = fgets(buffer, sizeof(buffer), stdin);
    if (!result) {
        fprintf(stderr, "[ERROR] Failed to read key.\n");
        return EXIT_FAILURE;
    }

    buffer[strcspn(buffer, "\n")] = 0;

    if (strlen(buffer) > 16) {
        fprintf(stderr, "[ERROR] Key cannot be more than 8 bytes.\n");
        return EXIT_FAILURE;
    }

    char* endptr = NULL;
    uint64_t key = strtoull(buffer, &endptr, 16);
    if (endptr == buffer) {
        fprintf(stderr, "[ERROR] Key is not a valid number.\n");
        return EXIT_FAILURE;
    }

    memset(buffer, 0, sizeof(buffer));

    printf("Enter plaintext: ");
    result = fgets(buffer, sizeof(buffer), stdin);
    if (!result) {
        fprintf(stderr, "[ERROR] Failed to read plaintext.\n");
        return EXIT_FAILURE;
    }

    buffer[strcspn(buffer, "\r\n")] = 0;

    des_encrypt(key, buffer);
    return EXIT_SUCCESS;
}
