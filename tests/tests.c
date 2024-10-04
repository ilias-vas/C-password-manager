#include <stdio.h>
#include <string.h>

#include "tests.h"
#include "../src/encryption/sha1.h"

void sha1_stringify(char data[SHA1_HASH_SIZE], char result[SHA1_HASH_SIZE * 2 + 1]) {
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
    sha1_stringify(result, result_string);
    passed += EXPECT_STRING(message, "c45597ef9e8a60438211dbe0eda6b7ea0b25cc1c", result_string);
    ++*total;

    message = "this 64-character string tests sha1 function internal block size";
    sha1_hash(message, strlen(message), result);
    sha1_stringify(result, result_string);
    passed += EXPECT_STRING(message, "313a5eeadd3df09a399c2dc75791850b54256015", result_string);
    ++*total;

    message = "this string is intentionally made longer than 64 characters to test sha1's internal block size behavior";
    sha1_hash(message, strlen(message), result);
    sha1_stringify(result, result_string);
    passed += EXPECT_STRING(message, "6035253b8569ee0b2bd0f4383b7bd884c08afb33", result_string);
    ++*total;

    return passed;
}

int main(void) {
    RUN_TEST(test_sha1);
    return 0;
}
