#ifndef SHA_1_H
#define SHA_1_H

#include "../util/defines.h"

#define SHA1_HASH_SIZE 20

typedef unsigned int uint32_t;

void sha1_hash(const char* data, size_t size, char result[SHA1_HASH_SIZE]);

#endif
