#ifndef ACCOUNT_H
#define ACCOUNT_H

#define MAX_NAME_LENGTH 64
#define MAX_PASSWORD_LENGTH 128

typedef struct account {
    char name[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} account_t;

account_t *createAccount(char *name, char *password);

#endif
