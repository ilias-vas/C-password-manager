#ifndef PBKDF2_H
#define PBKDF2_H

#include "aes.h"
#include "sha1.h"
#include "../util/defines.h"

#define SALT_LENGTH 16

void generate_salt(char* result, size_t size);
void PBKDF2(const char* password, const char salt[SALT_LENGTH], uint32_t* result, size_t size);

#endif
