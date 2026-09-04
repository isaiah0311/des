/**
 * \file main.c
 * \author Isaiah Lateer
 *
 * Entry point for the project.
 */

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "des.h"

/**
 * Processes arguments in order to determin whether to encrypt or decrypt and
 * which files to read from and write to.
 *
 * \param[in] argc Number of arguments.
 * \param[in] argv Arguments.
 * \return Exit code.
 */
int main(int argc, const char** argv) {
    enum direction {
        DIRECTION_UNDEFINED,
        DIRECTION_ENCRYPT,
        DIRECTION_DECRYPT
    };

    enum mode { MODE_UNDEFINED, MODE_ECB, MODE_CBC };

    bool should_exit = false;
    enum direction direction = DIRECTION_UNDEFINED;
    enum mode mode = MODE_UNDEFINED;
    uint64_t key = 0;
    bool set_key = false;
    uint64_t iv = 0;
    bool set_iv = false;
    FILE* in_file = NULL;
    FILE* out_file = NULL;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-d") == 0) {
            if (direction != DIRECTION_UNDEFINED) {
                fprintf(stderr,
                    "[ERROR] Multiple direction values were given.\n");
                should_exit = true;
                break;
            } else if (i + 1 < argc) {
                if (strcmp(argv[i + 1], "enc") == 0) {
                    direction = DIRECTION_ENCRYPT;
                } else if (strcmp(argv[i + 1], "dec") == 0) {
                    direction = DIRECTION_DECRYPT;
                } else {
                    fprintf(stderr, "[ERROR] Direction is invalid\n");
                    should_exit = true;
                    break;
                }

                ++i;
            } else {
                fprintf(stderr, "[ERROR] Missing direction value after -d.\n");
                should_exit = true;
                break;
            }
        } else if (strcmp(argv[i], "-m") == 0) {
            if (mode != MODE_UNDEFINED) {
                fprintf(stderr, "[ERROR] Multiple mode values were given.\n");
                should_exit = true;
                break;
            } else if (i + 1 < argc) {
                if (strcmp(argv[i + 1], "ecb") == 0) {
                    mode = MODE_ECB;
                } else if (strcmp(argv[i + 1], "cbc") == 0) {
                    mode = MODE_CBC;
                } else {
                    fprintf(stderr, "[ERROR] Mode is invalid\n");
                    should_exit = true;
                    break;
                }

                ++i;
            } else {
                fprintf(stderr, "[ERROR] Missing mode value after -m.\n");
                should_exit = true;
                break;
            }
        } else if (strcmp(argv[i], "-k") == 0) {
            if (set_key) {
                fprintf(stderr, "[ERROR] Multiple key values were given.\n");
                should_exit = true;
                break;
            } else if (i + 1 < argc) {
                char* endptr = NULL;
                key = strtoull(argv[i + 1], &endptr, 16);
                if (endptr == argv[i + 1]) {
                    fprintf(stderr, "[ERROR] Key is not a valid number.\n");
                    should_exit = true;
                    break;
                }

                set_key = true;
                ++i;
            } else {
                fprintf(stderr, "[ERROR] Missing key value after -k.\n");
                should_exit = true;
                break;
            }
        } else if (strcmp(argv[i], "-v") == 0) {
            if (set_iv) {
                fprintf(stderr, "[ERROR] Multiple IV values were given.\n");
                should_exit = true;
                break;
            } else if (i + 1 < argc) {
                char* endptr = NULL;
                iv = strtoull(argv[i + 1], &endptr, 16);
                if (endptr == argv[i + 1]) {
                    fprintf(stderr, "[ERROR] IV is not a valid number.\n");
                    should_exit = true;
                    break;
                }

                set_iv = true;
                ++i;
            } else {
                fprintf(stderr, "[ERROR] Missing IV value after -v.\n");
                should_exit = true;
                break;
            }
        } else if (strcmp(argv[i], "-i") == 0) {
            if (in_file) {
                fprintf(stderr,
                    "[ERROR] Multiple input file values were given.\n");
                should_exit = true;
                break;
            } else if (i + 1 < argc) {
                in_file = fopen(argv[i + 1], "rb");
                if (!in_file) {
                    fprintf(stderr, "[ERROR] Failed to open input file.\n");
                    should_exit = true;
                    break;
                }

                ++i;
            } else {
                fprintf(stderr, "[ERROR] Missing input file path after -i.\n");
                should_exit = true;
                break;
            }
        } else if (strcmp(argv[i], "-o") == 0) {
            if (out_file) {
                fprintf(stderr,
                    "[ERROR] Multiple output file values were given.\n");
                should_exit = true;
                break;
            } else if (i + 1 < argc) {
                out_file = fopen(argv[i + 1], "wb");
                if (!out_file) {
                    fprintf(stderr, "[ERROR] Failed to open output file.\n");
                    should_exit = true;
                    break;
                }

                ++i;
            } else {
                fprintf(stderr, "[ERROR] Missing output file path after -o.\n");
                should_exit = true;
                break;
            }
        } else {
            fprintf(stderr, "[WARN] Skipping unknown argument: %s\n", argv[i]);
        }
    }

    if (!should_exit) {
        if (!set_key) {
            fprintf(stderr, "[ERROR] No key was given.\n");
            should_exit = true;
        } else if (!in_file) {
            fprintf(stderr, "[ERROR] No input file was given.\n");
            should_exit = true;
        }
    }

    if (should_exit) {
        if (in_file) {
            fclose(in_file);
        }

        if (out_file) {
            fclose(out_file);
        }

        return EXIT_FAILURE;
    }

    if (mode == MODE_CBC && !set_iv) {
        srand((unsigned int) time(NULL));
        for (int i = 0; i < 4; ++i) {
            iv = (iv << 16) | (rand() & 0xFFFF);
        }

        printf("[INFO] Generated IV: 0x%016" PRIX64 "\n", iv);
    }

    long byte_count = 0;
    size_t bytes_written = 0;

    switch (direction) {
    case DIRECTION_UNDEFINED:
    case DIRECTION_ENCRYPT:
        fseek(in_file, 0, SEEK_END);
        byte_count = ((ftell(in_file) / 8) + 1) * 8;
        uint8_t* ciphertext = malloc(byte_count);
        if (!ciphertext) {
            fprintf(stderr,
                "[ERROR] Failed to allocated memory for the ciphertext "
                "buffer.\n");
            fclose(in_file);
            return EXIT_FAILURE;
        }

        switch (mode) {
        case MODE_UNDEFINED:
        case MODE_ECB:
            bytes_written = des_ecb_encrypt(key, in_file, byte_count,
                ciphertext);
            break;
        case MODE_CBC:
            bytes_written = des_cbc_encrypt(key, iv, in_file, byte_count,
                ciphertext);
            break;
        }

        if (out_file) {
            fwrite(ciphertext, sizeof(uint8_t), bytes_written, out_file);
        } else {
            printf("0x");
            for (size_t i = 0; i < bytes_written; ++i) {
                printf("%02X", ciphertext[i]);
            }

            printf("\n");
        }

        free(ciphertext);

        break;
    case DIRECTION_DECRYPT:
        fseek(in_file, 0, SEEK_END);
        byte_count = ftell(in_file);
        uint8_t* plaintext = malloc(byte_count);
        if (!plaintext) {
            fprintf(stderr,
                "[ERROR] Failed to allocated memory for the plaintext "
                "buffer.\n");
            fclose(in_file);
            return EXIT_FAILURE;
        }

        switch (mode) {
        case MODE_UNDEFINED:
        case MODE_ECB:
            bytes_written = des_ecb_decrypt(key, in_file, byte_count,
                plaintext);
            break;
        case MODE_CBC:
            bytes_written = des_cbc_decrypt(key, iv, in_file, byte_count,
                plaintext);
            break;
        }

        if (out_file) {
            fwrite(plaintext, sizeof(uint8_t), bytes_written, out_file);
        } else {
            printf("0x");
            for (size_t i = 0; i < bytes_written; ++i) {
                printf("%02X", plaintext[i]);
            }

            printf("\n");
        }

        free(plaintext);

        break;
    }

    fclose(in_file);
    if (out_file) {
        fclose(out_file);
    }

    return EXIT_SUCCESS;
}
