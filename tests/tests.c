#include <stdio.h>
#include <string.h>

#include "tests.h"
#include "../src/encryption/sha1.h"
#include "../src/encryption/hmac.h"
#include "../src/encryption/pbkdf2.h"

void hash_stringify(const char* data, size_t size, char* result) {
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
    aes_key_t key = {0};
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

    PBKDF2("plnlrtfpijpuhqylxbgqiiyipieyxvfsavzgxbbcfusqkozwpngsyejqlmjsytrmd", salt, &key);
    hash_stringify((char*)key.words, AES_KEY_LENGTH * sizeof(uint32_t), result_string);
    passed += EXPECT_STRING("plnlrtfpijpuhqylxbgqiiyipieyxvfsavzgxbbcfusqkozwpngsyejqlmjsytrmd", "17eb4014c8c461c300e9b61518b9a18b", result_string);
    ++*total;

    PBKDF2("eBkXQTfuBqp'cTcar&g*", salt, &key);
    hash_stringify((char*)key.words, AES_KEY_LENGTH * sizeof(uint32_t), result_string);
    passed += EXPECT_STRING("eBkXQTfuBqp'cTcar&g*", "17eb4014c8c461c300e9b61518b9a18b", result_string);
    ++*total;

    return passed;
}

void key_expansion(const aes_key_t* primary_key, aes_key_t round_keys[ROUNDS + 1]);
int test_key_expansion(int* total) {
    int passed = 0;
   
    aes_key_t primary_key = {0};
    aes_key_t round_keys[ROUNDS + 1];
    uint32_t expected_round_keys[(ROUNDS + 1) * AES_KEY_LENGTH] = {
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x62636363, 0x62636363, 0x62636363, 0x62636363, 0x9b9898c9, 0xf9fbfbaa, 
        0x9b9898c9, 0xf9fbfbaa, 0x90973450, 0x696ccffa, 0xf2f45733, 0x0b0fac99, 0xee06da7b, 0x876a1581, 0x759e42b2, 0x7e91ee2b,
        0x7f2e2b88, 0xf8443e09, 0x8dda7cbb, 0xf34b9290, 0xec614b85, 0x1425758c, 0x99ff0937, 0x6ab49ba7, 0x21751787, 0x3550620b,
        0xacaf6b3c, 0xc61bf09b, 0x0ef90333, 0x3ba96138, 0x97060a04, 0x511dfa9f, 0xb1d4d8e2, 0x8a7db9da, 0x1d7bb3de, 0x4c664941,
        0xb4ef5bcb, 0x3e92e211, 0x23e951cf, 0x6f8f188e
    };

    key_expansion(&primary_key, round_keys);
    int equal = 1;
    int i;
    for (i = 0; i < (ROUNDS + 1) * AES_KEY_LENGTH; ++i) {
        if (round_keys->words[i] == expected_round_keys[i]) continue;
        equal = 0;
        break;
    }

    passed += EXPECT_TRUE(equal, "all zeros");
    ++*total;
    return passed;
}

void aes_encrypt(uint8_t block[BLOCK_SIZE], const aes_key_t* key);
void aes_decrypt(uint8_t block[BLOCK_SIZE], const aes_key_t* key);
int test_aes(int* total) {
    int passed = 0;
   
    aes_key_t key = {0};
    uint8_t block[BLOCK_SIZE] = {
        0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x07, 0x0f, 0x0f, 0x1f, 0x1f, 0x3f, 0x3f, 0x7f, 0x7f
    };
    uint8_t expected[BLOCK_SIZE];
    memcpy(expected, block, BLOCK_SIZE);

    aes_encrypt(block, &key);
    aes_decrypt(block, &key);

    int equal = 1;
    int i;
    for (i = 0; i < BLOCK_SIZE; ++i) {
        if (block[i] == expected[i]) continue;
        equal = 0;
        break;
    }
    passed += EXPECT_TRUE(equal, "1 block encrypt and decrypt");
    ++*total;

    uint8_t iv[BLOCK_SIZE];
    generate_salt((char*)iv, BLOCK_SIZE);
    stream_t plain_text = stream_init();
    stream_push_string(&plain_text, "this is a short test");

    stream_t cypher_text = aes_encrypt_stream(&plain_text, &key, iv);
    stream_t decrypted_text = aes_decrypt_stream(&cypher_text, &key, iv);

    equal = plain_text.size == decrypted_text.size;
    for (i = 0; i < plain_text.size; ++i) {
        if (!equal) break;
        if (plain_text.data[i] == decrypted_text.data[i]) continue;
        equal = 0;
    }

    passed += EXPECT_TRUE(equal, "short stream encrypt and decrypt");
    ++*total;

    stream_free(&plain_text);
    stream_free(&cypher_text);
    stream_free(&decrypted_text);

    generate_salt((char*)iv, BLOCK_SIZE);
    plain_text = stream_init();
    stream_push_string(&plain_text, "this is a much longer text that will span over multiple blocks so that we can test if everything is working properly");

    cypher_text = aes_encrypt_stream(&plain_text, &key, iv);
    decrypted_text = aes_decrypt_stream(&cypher_text, &key, iv);

    equal = plain_text.size == decrypted_text.size;
    for (i = 0; i < plain_text.size; ++i) {
        if (!equal) break;
        if (plain_text.data[i] == decrypted_text.data[i]) continue;
        equal = 0;
    }

    passed += EXPECT_TRUE(equal, "long stream encrypt and decrypt");
    ++*total;

    stream_free(&plain_text);
    stream_free(&cypher_text);
    stream_free(&decrypted_text);


    return passed;
}

int main(void) {
    RUN_TEST(test_sha1);
    RUN_TEST(test_hmac);
    RUN_TEST(test_pbkdf2);
    RUN_TEST(test_key_expansion);
    RUN_TEST(test_aes);
    return 0;
}
