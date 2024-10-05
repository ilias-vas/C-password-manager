#ifndef HMAC_H
#define HMAC_H

#include "sha1.h"

void HMAC_SHA1(const char* key, const char* message, size_t size, char result[SHA1_HASH_SIZE]);

#endif
