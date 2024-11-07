#include "sha1.h"

#include <string.h>
#include <stdlib.h>

#define CHUNK_SIZE 64
#define TOTAL_WORDS 80

#define H0 0x67452301
#define H1 0xEFCDAB89
#define H2 0x98BADCFE
#define H3 0x10325476
#define H4 0xC3D2E1F0

#define K0 0x5A827999
#define K1 0x6ED9EBA1
#define K2 0x8F1BBCDC
#define K3 0xCA62C1D6

typedef struct {
    uint32_t h0, h1, h2, h3, h4;
} sha1_state_t;

void process_chunk(sha1_state_t* state, unsigned char chunk[CHUNK_SIZE]) {
    uint32_t* words = (uint32_t*) malloc(TOTAL_WORDS * sizeof(uint32_t));
    memcpy(words, chunk, CHUNK_SIZE); /* fill in the first 16 words */

    int i;
    /* swap the bits here so that we are working with big endian values */
    for (i = 0; i < 16; ++i) words[i] = __builtin_bswap32(words[i]);
    for (; i < TOTAL_WORDS; ++i) {
        words[i] = words[i - 3] ^ words[i - 8] ^ words[i - 14] ^ words[i - 16];
        words[i] = ROTATE_LEFT(words[i], 1);
    }

    uint32_t a, b, c, d, e, f, k, t;
    a = state->h0;
    b = state->h1;
    c = state->h2;
    d = state->h3;
    e = state->h4;

    for (i = 0; i < TOTAL_WORDS; ++i) {
        if (i < 20) {
            f = (b & c) | (~b & d);
            k = K0;
        } else if (i < 40) {
            f = b ^ c ^ d;
            k = K1;
        } else if (i < 60) {
            f = (b & c) | (b & d) | (c & d);
            k = K2;
        } else {
            f = b ^ c ^ d;
            k = K3;
        }

        t = ROTATE_LEFT(a, 5) + f + e + k + words[i];
        e = d;
        d = c;
        c = ROTATE_LEFT(b, 30);
        b = a;
        a = t;
    }

    state->h0 += a;
    state->h1 += b;
    state->h2 += c;
    state->h3 += d;
    state->h4 += e;
    free(words);
}

void sha1_hash(const char* data, size_t size, char result[SHA1_HASH_SIZE]) {
    size_t chunks = ((size + 1) / CHUNK_SIZE + 1);
    size_t block_size = chunks * CHUNK_SIZE;

    /* padding */
    unsigned char* block = (unsigned char*) calloc(block_size, sizeof(char));
    memcpy(block, data, size);
    block[size] = 0x80;

    /* this assumes that the platform is little endian */
    /* swap to big endian */
    size_t length = __builtin_bswap64(size * 8);
    memcpy(block + block_size - sizeof(length), &length, sizeof(length));

    sha1_state_t state;
    state.h0 = H0;
    state.h1 = H1;
    state.h2 = H2;
    state.h3 = H3;
    state.h4 = H4;

    int i;
    for (i = 0; i < chunks; ++i) process_chunk(&state, block + i * CHUNK_SIZE);

    /* revert the output back to little endian */
    ((uint32_t*) result)[0] = __builtin_bswap32(state.h0);
    ((uint32_t*) result)[1] = __builtin_bswap32(state.h1);
    ((uint32_t*) result)[2] = __builtin_bswap32(state.h2);
    ((uint32_t*) result)[3] = __builtin_bswap32(state.h3);
    ((uint32_t*) result)[4] = __builtin_bswap32(state.h4);
    free(block);
}
