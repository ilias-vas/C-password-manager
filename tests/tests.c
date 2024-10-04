#include <stdio.h>
#include <string.h>

#include "tests.h"
#include "../src/encryption/sha1.h"
#include "../src/encryption/hmac.h"

void hash_stringify(char data[SHA1_HASH_SIZE], char result[SHA1_HASH_SIZE * 2 + 1]) {
    int i;
    for(i = 0; i < SHA1_HASH_SIZE; ++i) {
        sprintf(result + i * 2, "%02x", (unsigned char) data[i]);
    }
}

int test_sha1(int* total) {
    int passed = 0;
    char result[SHA1_HASH_SIZE];
    char result_string[SHA1_HASH_SIZE * 2 + 1];
    char* message;

    message = "sha1 implementation";
    sha1_hash(message, strlen(message), result);
    hash_stringify(result, result_string);
    passed += EXPECT_STRING(message, "c45597ef9e8a60438211dbe0eda6b7ea0b25cc1c", result_string);
    ++*total;

    message = "this 64-character string tests sha1 function internal block size";
    sha1_hash(message, strlen(message), result);
    hash_stringify(result, result_string);
    passed += EXPECT_STRING(message, "313a5eeadd3df09a399c2dc75791850b54256015", result_string);
    ++*total;

    message = "this string is intentionally made longer than 64 characters to test sha1's internal block size behavior";
    sha1_hash(message, strlen(message), result);
    hash_stringify(result, result_string);
    passed += EXPECT_STRING(message, "6035253b8569ee0b2bd0f4383b7bd884c08afb33", result_string);
    ++*total;

    return passed;
}

int test_hmac(int* total) {
    int passed = 0;
    char result[SHA1_HASH_SIZE];
    char result_string[SHA1_HASH_SIZE * 2 + 1];
    char* key;
    char* message;

    key = "key";
    message = "The quick brown fox jumps over the lazy dog";
    HMAC_SHA1(key, message, result);
    hash_stringify(result, result_string);
    passed += EXPECT_STRING(message, "de7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9", result_string);
    ++*total;

    key = "this here is also a very large key so that we can test the key hashing part of hmac";
    message = "this is some random other string that is really long just to test how it responds to very long inputs";
    HMAC_SHA1(key, message, result);
    hash_stringify(result, result_string);
    passed += EXPECT_STRING(message, "87669a5d14e298181a6f911ce171b3d1f407e844", result_string);
    ++*total;

    return passed;
}

int main(void) {
    RUN_TEST(test_sha1);
    RUN_TEST(test_hmac);
    return 0;
}
