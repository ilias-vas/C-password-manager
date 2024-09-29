#include "vault.h"
#include "stream.h"

#include <stdio.h>
#include <string.h>
#include <stdio.h>

#define MAGIC_WORD "pmanvault"

int vault_exists(vault_t* vault) {
    FILE* file = fopen(vault->file_path, "r");
    if (file == NULL) return 0;
    fclose(file);
    return 1;
}

int vault_load(vault_t* vault, const char* password) {
    stream_t stream = stream_init();
    if (!stream_read_from_file(&stream, vault->file_path)) {
        stream_free(&stream);
        return 0;
    }

    char magic_word[sizeof(MAGIC_WORD)];
    stream_pop_string(&stream, magic_word);
    if (strcmp(magic_word, MAGIC_WORD)) {
        stream_free(&stream);
        return 0;
    }

    stream_pop_string(&stream, vault->username);
    vault->root = stream_pop_category(&stream);
    stream_free(&stream);
    return 1;
}

int vault_save(vault_t* vault, const char* password) {
    stream_t stream = stream_init();
    stream_push_string(&stream, MAGIC_WORD);
    stream_push_string(&stream, vault->username);
    stream_push_category(&stream, vault->root);
    int status = stream_save_to_file(&stream, vault->file_path);
    stream_free(&stream);

    return status;
}
