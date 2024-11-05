#ifndef AES_H
#define AES_H

#include "../util/defines.h"

#define BLOCK_SIZE 16 /* 16 bytes -> 128 bits */
#define AES_KEY_LENGTH 4 /* 4 words -> 128 bits*/
#define ROUNDS 10
typedef unsigned char uint8_t;

typedef struct {
    uint32_t words[AES_KEY_LENGTH];
} aes_key_t;

#endif
