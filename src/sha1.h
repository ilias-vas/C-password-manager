#ifndef SHA_1_H
#define SHA_1_H

#include "defines.h"

void sha1_hash(const char* data, size_t size, unsigned char result[20]);

#endif
