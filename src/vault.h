#ifndef VAULT_H
#define VAULT_H

#include "defines.h"
#include "account.h"

typedef struct {
    const char* file_path;
    char username[MAX_NAME_SIZE];
    char password[MAX_PASSWORD_SIZE];
    category_t* root;
} vault_t;

int vault_exists(vault_t*);
int vault_load(vault_t*);
int vault_save(vault_t*);

#endif
