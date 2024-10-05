#include "hmac.h"

#include <string.h>
#include <stdlib.h>

#define BLOCK_SIZE 64

void HMAC_SHA1(const char* key, const char* message, size_t size, char result[SHA1_HASH_SIZE]) {
    char key_prime[BLOCK_SIZE];
    memset(key_prime, 0, BLOCK_SIZE);

    if (strlen(key) <= BLOCK_SIZE) strcpy(key_prime, key);
    else sha1_hash(key, strlen(key), key_prime);

    char inner_padding[BLOCK_SIZE];
    char outer_padding[BLOCK_SIZE];
    memset(inner_padding, 0x36, BLOCK_SIZE);
    memset(outer_padding, 0x5C, BLOCK_SIZE);

    int i;
    for (i = 0; i < BLOCK_SIZE; ++i) {
        inner_padding[i] ^= key_prime[i];
        outer_padding[i] ^= key_prime[i];
    }

    char* inner_hash = (char*) malloc(BLOCK_SIZE + size);
    memcpy(inner_hash, inner_padding, BLOCK_SIZE);
    memcpy(inner_hash + BLOCK_SIZE, message, size);
    sha1_hash(inner_hash, BLOCK_SIZE + size, result);

    char* outer_hash = (char*) malloc(BLOCK_SIZE + SHA1_HASH_SIZE);
    memcpy(outer_hash, outer_padding, BLOCK_SIZE);
    memcpy(outer_hash + BLOCK_SIZE, result, SHA1_HASH_SIZE);
    sha1_hash(outer_hash, BLOCK_SIZE + SHA1_HASH_SIZE, result);

    free(inner_hash);
    free(outer_hash);
}
