#ifndef AES_H
#define AES_H

#include "../util/defines.h"
#include "../util/stream.h"

#define BLOCK_SIZE 16 /* 16 bytes -> 128 bits */
#define AES_KEY_LENGTH 4 /* 4 words -> 128 bits*/
#define ROUNDS 10
typedef unsigned char uint8_t;

typedef struct {
    uint32_t words[AES_KEY_LENGTH];
} aes_key_t;

stream_t aes_encrypt_stream(stream_t* stream, const aes_key_t* key, const uint8_t iv[BLOCK_SIZE]);
stream_t aes_decrypt_stream(stream_t* stream, const aes_key_t* key, const uint8_t iv[BLOCK_SIZE]);

#endif
