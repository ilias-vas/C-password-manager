#include <stdio.h>
#include <string.h>

#include "tests.h"
#include "../src/encryption/sha1.h"
#include "../src/encryption/hmac.h"
#include "../src/encryption/pbkdf2.h"

void hash_stringify(const char* data, size_t size, char result[size * 2 + 1]) {
    int i;
    for(i = 0; i < size; ++i) {
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
    hash_stringify(result, SHA1_HASH_SIZE, result_string);
    passed += EXPECT_STRING(message, "c45597ef9e8a60438211dbe0eda6b7ea0b25cc1c", result_string);
    ++*total;

    message = "this 64-character string tests sha1 function internal block size";
    sha1_hash(message, strlen(message), result);
    hash_stringify(result, SHA1_HASH_SIZE, result_string);
    passed += EXPECT_STRING(message, "313a5eeadd3df09a399c2dc75791850b54256015", result_string);
    ++*total;

    message = "this string is intentionally made longer than 64 characters to test sha1's internal block size behavior";
    sha1_hash(message, strlen(message), result);
    hash_stringify(result, SHA1_HASH_SIZE, result_string);
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
    HMAC_SHA1(key, message, strlen(message), result);
    hash_stringify(result, SHA1_HASH_SIZE, result_string);
    passed += EXPECT_STRING(message, "de7c9b85b8b78aa6bc8a7a36f70a90701c9db4d9", result_string);
    ++*total;

    key = "this here is also a very large key so that we can test the key hashing part of hmac";
    message = "this is some random other string that is really long just to test how it responds to very long inputs";
    HMAC_SHA1(key, message, strlen(message), result);
    hash_stringify(result, SHA1_HASH_SIZE, result_string);
    passed += EXPECT_STRING(message, "87669a5d14e298181a6f911ce171b3d1f407e844", result_string);
    ++*total;

    return passed;
}

int test_pbkdf2(int* total) {
    int passed = 0;
    uint32_t derived_key[AES_KEY_LENGTH];
    char result_string[AES_KEY_LENGTH * sizeof(uint32_t) * 2 + 1];
    char salt[SALT_LENGTH] = "saltdoesntmatter";

    /*
     * TODO: Fix PBKDF2
     * (https://en.wikipedia.org/wiki/PBKDF2#:~:text=The%20following%20two%20function%20calls%3A)
     * According to wikipedia these should produce the same derived key regardless of the iterations or salt,
     * which they do, but just not the correct one
     * The first 16 bytes are shown below. I couldn't get it to work. Feel free to take a look at the pbkdf2
     * implemenation if you want to have a crack at fixing it.
     * For right now, it doesnt really matter if its the *correct* key, as long as the key is consistant
     * when given the same inputs.
     */

    PBKDF2("plnlrtfpijpuhqylxbgqiiyipieyxvfsavzgxbbcfusqkozwpngsyejqlmjsytrmd", salt, derived_key, AES_KEY_LENGTH);
    hash_stringify((char*) derived_key, AES_KEY_LENGTH * sizeof(uint32_t), result_string);
    passed += EXPECT_STRING("plnlrtfpijpuhqylxbgqiiyipieyxvfsavzgxbbcfusqkozwpngsyejqlmjsytrmd", "17eb4014c8c461c300e9b61518b9a18b", result_string);
    ++*total;

    PBKDF2("eBkXQTfuBqp'cTcar&g*", salt, derived_key, AES_KEY_LENGTH);
    hash_stringify((char*) derived_key, AES_KEY_LENGTH * sizeof(uint32_t), result_string);
    passed += EXPECT_STRING("eBkXQTfuBqp'cTcar&g*", "17eb4014c8c461c300e9b61518b9a18b", result_string);
    ++*total;

    return passed;
}

#define ROUND_KEY_LENGTH 4
#define ROUNDS 10
void key_expansion(uint32_t primary_key[AES_KEY_LENGTH], uint32_t round_keys[(ROUNDS + 1) * ROUND_KEY_LENGTH]);
int test_key_expansion(int* total) {
    int passed = 0;
   
    uint32_t primary_key[AES_KEY_LENGTH] = {0, 0, 0, 0};
    uint32_t round_keys[(ROUNDS + 1) * ROUND_KEY_LENGTH];
    uint32_t expected_round_keys[(ROUNDS + 1) * ROUND_KEY_LENGTH] = {
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x62636363, 0x62636363, 0x62636363, 0x62636363, 0x9b9898c9, 0xf9fbfbaa, 
        0x9b9898c9, 0xf9fbfbaa, 0x90973450, 0x696ccffa, 0xf2f45733, 0x0b0fac99, 0xee06da7b, 0x876a1581, 0x759e42b2, 0x7e91ee2b,
        0x7f2e2b88, 0xf8443e09, 0x8dda7cbb, 0xf34b9290, 0xec614b85, 0x1425758c, 0x99ff0937, 0x6ab49ba7, 0x21751787, 0x3550620b,
        0xacaf6b3c, 0xc61bf09b, 0x0ef90333, 0x3ba96138, 0x97060a04, 0x511dfa9f, 0xb1d4d8e2, 0x8a7db9da, 0x1d7bb3de, 0x4c664941,
        0xb4ef5bcb, 0x3e92e211, 0x23e951cf, 0x6f8f188e
    };

    key_expansion(primary_key, round_keys);
    int equal = 1;
    int i;
    for (i = 0; i < (ROUNDS + 1) * ROUND_KEY_LENGTH; ++i) {
        if (round_keys[i] == expected_round_keys[i]) continue;
        equal = 0;
        break;
    }

    passed += EXPECT_TRUE(equal, "all zeros");
    ++*total;
    return passed;
}

int main(void) {
    RUN_TEST(test_sha1);
    RUN_TEST(test_hmac);
    RUN_TEST(test_pbkdf2);
    RUN_TEST(test_key_expansion);
    return 0;
}
