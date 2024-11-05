#ifndef AES_H
#define AES_H

#include "../util/defines.h"

#define AES_KEY_LENGTH 4 /* 128, 192 or 256 bits */
#define BLOCK_SIZE 4
#define ROUND_KEY_LENGTH 4
#define ROUNDS 10
typedef unsigned char uint8_t;

typedef struct {
    uint32_t words[AES_KEY_LENGTH];
} aes_key_t;

#endif
