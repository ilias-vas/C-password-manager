#include "pbkdf2.h"

#include <stdio.h>
#include <string.h>
#include "hmac.h"
#include "sha1.h"

#define ASCII "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
#define PBKDF2_ITERATIONS 2048

void generate_salt(char* result, size_t size) {
    FILE* file = fopen("/dev/urandom", "r");
    fread(result, sizeof(char), size, file);
    fclose(file);

    const char* ascii = ASCII;
    size_t ascii_size = sizeof(ASCII) - 1;
    int i;
    for (i = 0; i < size; ++i) result[i] = ascii[result[i] % ascii_size];
}

void chain_hashes(const char* password, const char salt[SALT_LENGTH], uint32_t n, char result[SHA1_HASH_SIZE]) {
    n = __builtin_bswap32(n + 1); /* requires big endian 1-based indexing */

    size_t message_size = SALT_LENGTH + sizeof(uint32_t);
    char message[message_size];
    memcpy(message, salt, SALT_LENGTH);
    memcpy(message + SALT_LENGTH, &n, sizeof(uint32_t));
    HMAC_SHA1(password, message, message_size, result);

    char u[SHA1_HASH_SIZE];
    int i, j;
    /* i = 1 as the first iteration was done above */
    for (i = 1; i < PBKDF2_ITERATIONS; ++i) {
        HMAC_SHA1(password, result, SHA1_HASH_SIZE, u);
        for (j = 0; j < SHA1_HASH_SIZE; ++j) result[j] ^= u[j];
    }
}

aes_key_t PBKDF2(const char* password, const char salt[SALT_LENGTH]) {
    char t[SHA1_HASH_SIZE];
    int size = AES_KEY_LENGTH;
    int i, total_iterations = size * sizeof(uint32_t) / SHA1_HASH_SIZE + 1;
    aes_key_t key = {0};
    uint32_t* result = key.words;

    for (i = 0; i < total_iterations; ++i) {
        chain_hashes(password, salt, i, t);

        /* calculate the copy size in bytes */
        size_t copy_size = (size * sizeof(uint32_t) < SHA1_HASH_SIZE) ? size * sizeof(uint32_t) : SHA1_HASH_SIZE;
        memcpy(result, t, copy_size);

        /* move the result pointer to the next block to be copied, and decrease the size by the amount copied */
        size -= copy_size / sizeof(uint32_t);
        result += copy_size / sizeof(uint32_t);
    }
    return key;
}
