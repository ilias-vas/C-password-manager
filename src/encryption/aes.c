#include "aes.h"

#include "sha1.h"
#include <string.h>
#include <stdlib.h>

/* https://en.wikipedia.org/wiki/Rijndael_MixColumns */
#define GMUL2(value)  ((value << 1) ^ ((value & 0x80) ? 0x1b : 0x00))
#define GMUL3(value)  (GMUL2(value) ^ value)
#define GMUL9(value)  ((GMUL2(GMUL2(GMUL2(value)))) ^ (value))
#define GMUL11(value) ((GMUL2(GMUL2(GMUL2(value))) ^ GMUL2(value)) ^ (value))
#define GMUL13(value) ((GMUL2(GMUL2(GMUL2(value))) ^ GMUL2(GMUL2(value))) ^ (value))
#define GMUL14(value) ((GMUL2(GMUL2(GMUL2(value))) ^ GMUL2(GMUL2(value))) ^ GMUL2(value))


const uint8_t sbox[265] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, 
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, 
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, 
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

uint8_t inv_sbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, 
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, 
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, 
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, 
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, 
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, 
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, 
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, 
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, 
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, 
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, 
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d 
};

uint8_t round_constants[ROUNDS + 1];

void generate_round_constants() {
    round_constants[1] = 0x01;
    int i;
    for (i = 2; i < ROUNDS + 1; ++i) {
        round_constants[i] = round_constants[i - 1] * 2;
        if (round_constants[i - 1] & 0x80) round_constants[i] ^= 0x1b;
    } 
}

uint32_t sub_word(uint32_t word) {
    uint8_t* ptr = (uint8_t*)&word;

    /* sub each of the bytes in the word */
    int i;
    for (i = 0; i < 4; ++i) *(ptr + i) = sbox[*(ptr + i)];

    return word;
}

void key_expansion(const aes_key_t* primary_key, aes_key_t round_keys[ROUNDS + 1]) {
    /* first round key is the primary key */
    generate_round_constants();
    uint32_t round_key_words[(ROUNDS + 1) * AES_KEY_LENGTH];
    memcpy(round_key_words, primary_key->words, AES_KEY_LENGTH * sizeof(uint32_t));

    uint32_t temp;
    int i;

    /* loop over the keys but skip the primary key */
    for (i = AES_KEY_LENGTH; i < (ROUNDS + 1) * AES_KEY_LENGTH; ++i) {
        temp = round_key_words[i - 1];

        if (i % AES_KEY_LENGTH == 0) {
            temp = sub_word(ROTATE_LEFT(temp, 8)) ^ (round_constants[i / AES_KEY_LENGTH] << 24);
        }

        round_key_words[i] = round_key_words[i - AES_KEY_LENGTH] ^ temp;
    }

    memcpy(round_keys, round_key_words, (ROUNDS + 1) * AES_KEY_LENGTH * sizeof(uint32_t));
}

void shift_rows(uint8_t block[BLOCK_SIZE]) {
    uint8_t temp;

    /* nothing happens to row 0*/
    /* row 1 shift by 1 byte */
    temp = block[1];
    block[1] = block[5];
    block[5] = block[9];
    block[9] = block[13];
    block[13] = temp;

    /* row 2 shift by 2 bytes */
    temp = block[2];
    block[2] = block[10];
    block[10] = temp;
    temp = block[6];
    block[6] = block[14];
    block[14] = temp;

    /* row 3 shift by 3 bytes */
    temp = block[3];
    block[3] = block[15];
    block[15] = block[11];
    block[11] = block[7];
    block[7] = temp;
}

void mix_columns(uint8_t block[BLOCK_SIZE]) {
    int column;
    for (column = 0; column < 4; ++column) {
        uint8_t s0 = block[column];
        uint8_t s1 = block[4 + column];
        uint8_t s2 = block[8 + column];
        uint8_t s3 = block[12 + column];

        block[column]      = GMUL2(s0) ^ GMUL3(s1) ^ s2 ^ s3;
        block[4 + column]  = s0 ^ GMUL2(s1) ^ GMUL3(s2) ^ s3;
        block[8 + column]  = s0 ^ s1 ^ GMUL2(s2) ^ GMUL3(s3);
        block[12 + column] = GMUL3(s0) ^ s1 ^ s2 ^ GMUL2(s3);
    }
}

void add_round_key(uint8_t block[BLOCK_SIZE], const aes_key_t* key) {
    uint8_t* bytes = (uint8_t*) key->words;
    int i;
    for (i = 0; i < BLOCK_SIZE; ++i) block[i] ^= bytes[i];
}


void aes_encrypt(uint8_t block[BLOCK_SIZE], const aes_key_t* key) {
    aes_key_t round_keys[ROUNDS + 1];
    key_expansion(key, round_keys);

    add_round_key(block, &round_keys[0]);

    int i, j;
    for (i = 1; i < ROUNDS; ++i) {
        for (j = 0; j < BLOCK_SIZE; ++j) block[j] = sbox[block[j]];
        shift_rows(block);  
        mix_columns(block);
        add_round_key(block, &round_keys[i]);
    }

    for (j = 0; j < BLOCK_SIZE; ++j) block[j] = sbox[block[j]];
    shift_rows(block);
    add_round_key(block, &round_keys[ROUNDS]);
}

void inv_shift_rows(uint8_t block[BLOCK_SIZE]) {
    uint8_t temp;

    /* nothing happens to row 0 */
    /* row 1 shift by 1 byte in the opposite direction */
    temp = block[13];
    block[13] = block[9];
    block[9] = block[5];
    block[5] = block[1];
    block[1] = temp;

    /* row 2 shift by 2 bytes (same as encryption) */
    temp = block[2];
    block[2] = block[10];
    block[10] = temp;
    temp = block[6];
    block[6] = block[14];
    block[14] = temp;

    /* row 3 shift by 3 bytes in the opposite direction */
    temp = block[3];
    block[3] = block[7];
    block[7] = block[11];
    block[11] = block[15];
    block[15] = temp;
}

void inv_mix_columns(uint8_t block[BLOCK_SIZE]) {
    int column;
    for (column = 0; column < 4; ++column) {
        uint8_t s0 = block[column];
        uint8_t s1 = block[4 + column];
        uint8_t s2 = block[8 + column];
        uint8_t s3 = block[12 + column];

        block[column]      = GMUL14(s0) ^ GMUL11(s1) ^ GMUL13(s2) ^ GMUL9(s3);
        block[4 + column]  = GMUL9(s0) ^ GMUL14(s1) ^ GMUL11(s2) ^ GMUL13(s3);
        block[8 + column]  = GMUL13(s0) ^ GMUL9(s1) ^ GMUL14(s2) ^ GMUL11(s3);
        block[12 + column] = GMUL11(s0) ^ GMUL13(s1) ^ GMUL9(s2) ^ GMUL14(s3);
    }
}

void aes_decrypt(uint8_t block[BLOCK_SIZE], const aes_key_t* key) {
    aes_key_t round_keys[ROUNDS + 1];
    key_expansion(key, round_keys);

    int i, j;
    add_round_key(block, &round_keys[ROUNDS]);
    inv_shift_rows(block);
    for (j = 0; j < BLOCK_SIZE; ++j) block[j] = inv_sbox[block[j]];

    for (i = ROUNDS - 1; i > 0; --i) {
        add_round_key(block, &round_keys[i]);
        inv_mix_columns(block);
        inv_shift_rows(block);
        for (j = 0; j < BLOCK_SIZE; ++j) block[j] = inv_sbox[block[j]];
    }

    add_round_key(block, &round_keys[0]);
}

/* pkcs7 padding adds the value of the total length of padded bytes (minimum of 1 maximum of BLOCK_SIZE)
*  to the end of the original data aligned to BLOCK_SIZE blocks
*/
void pkcs7_pad(uint8_t* data, size_t data_len, size_t padded_len) {
    uint8_t padding_value = padded_len - data_len;
    int i;
    for (i = data_len; i < padded_len; ++i) data[i] = padding_value;
}

size_t pkcs7_unpad(uint8_t* data, size_t data_len) {
    uint8_t padding_value = data[data_len - 1];
    return data_len - padding_value;
}

void xor_block(uint8_t* block, const uint8_t* other) {
    int i;
    for (i = 0; i < BLOCK_SIZE; ++i) block[i] ^= other[i];
}

/* We are using Cypher block chaining (cbc) AES encryption
*  https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Cipher_block_chaining_(CBC)
*/

stream_t aes_encrypt_stream(stream_t* stream, const aes_key_t* key, const uint8_t iv[BLOCK_SIZE]) {
    /* create a padded buffer aligned to BLOCK_SIZE */
    size_t padded_len = ((stream->size + BLOCK_SIZE - 1) / BLOCK_SIZE) * BLOCK_SIZE;
    uint8_t* buffer = (uint8_t*) malloc(padded_len);
    memcpy(buffer, stream->data, stream->size);
    pkcs7_pad(buffer, stream->size, padded_len);

    /* an array of random bytes is used so that the same input will produce unique outputs every time */
    uint8_t previous_block[BLOCK_SIZE];
    memcpy(previous_block, iv, BLOCK_SIZE);

    stream_t cypher = stream_init();
    int i;
    for (i = 0; i < padded_len; i += BLOCK_SIZE) {
        uint8_t* current_block = buffer + i;
        xor_block(current_block, previous_block);
        aes_encrypt(current_block, key);

        memcpy(previous_block, current_block, BLOCK_SIZE); 
        stream_push(&cypher, current_block, BLOCK_SIZE);
    }

    free(buffer);
    return cypher;
}

stream_t aes_decrypt_stream(stream_t* stream, const aes_key_t* key, const uint8_t iv[BLOCK_SIZE]) {
    uint8_t current_block[BLOCK_SIZE];
    uint8_t previous_block[BLOCK_SIZE];

    memcpy(current_block, iv, BLOCK_SIZE);

    stream_t plain = stream_init();
    int i;
    for (i = 0; i < stream->size; i += BLOCK_SIZE) {
        memcpy(previous_block, current_block, BLOCK_SIZE);
        memcpy(current_block, stream->data + i, BLOCK_SIZE);

        aes_decrypt(current_block, key);
        xor_block(current_block, previous_block);
        stream_push(&plain, current_block, BLOCK_SIZE);

        /* we are xoring with the encrypted block so we need to use that for the next one*/
        memcpy(current_block, stream->data + i, BLOCK_SIZE);
    }

    /* remove the padding from the end */
    size_t unpadded_len = pkcs7_unpad((uint8_t*)plain.data, plain.size);
    
    plain.size = unpadded_len;
    return plain;
}