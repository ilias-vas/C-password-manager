#include "vault.h"
#include "util/stream.h"
#include "encryption/pbkdf2.h"
#include "encryption/aes.h"

#include <stdio.h>
#include <string.h>
#include <stdio.h>

const uint8_t MAGIC[2] = {'p', 'v'};

int vault_exists(vault_t* vault) {
    FILE* file = fopen(vault->file_path, "r");
    if (file == NULL) return 0;
    fclose(file);
    return 1;
}

int vault_load(vault_t* vault) {
    stream_t stream = stream_init();
    if (!stream_read_from_file(&stream, vault->file_path)) {
        stream_free(&stream);
        return 0;
    }

    uint8_t* iv = (uint8_t*) stream.data + stream.size - SALT_LENGTH - BLOCK_SIZE;
    char* salt = stream.data + stream.size - SALT_LENGTH;
    stream.size -= SALT_LENGTH + BLOCK_SIZE;

    aes_key_t key = PBKDF2(vault->password, salt);
    stream_t unencrypted = aes_decrypt_stream(&stream, &key, iv);
    unencrypted.it = unencrypted.data;

    uint8_t* magic = stream_pop(&unencrypted, 2);
    if (magic[0] != MAGIC[0] || magic[1] != MAGIC[1]) {
        stream_free(&unencrypted);
        stream_free(&stream);
        return 0;
    }

    stream_pop_string(&unencrypted, vault->username);
    vault->root = stream_pop_category(&unencrypted);
    stream_free(&unencrypted);
    stream_free(&stream);
    return 1;
}

int vault_save(vault_t* vault) {
    stream_t stream = stream_init();
    stream_push(&stream, MAGIC, 2);
    stream_push_string(&stream, vault->username);
    stream_push_category(&stream, vault->root);

    uint8_t iv[BLOCK_SIZE];
    char salt[SALT_LENGTH];
    generate_salt((char*) iv, BLOCK_SIZE);
    generate_salt(salt, SALT_LENGTH);

    aes_key_t key = PBKDF2(vault->password, salt);

    stream_t encrypted = aes_encrypt_stream(&stream, &key, iv);
    stream_push(&encrypted, iv, BLOCK_SIZE);
    stream_push(&encrypted, salt, SALT_LENGTH);
    int status = stream_save_to_file(&encrypted, vault->file_path);

    stream_free(&stream);
    stream_free(&encrypted);
    return status;
}
