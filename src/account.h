#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "list.h"
#include "defines.h"

typedef struct {
    char name[MAX_NAME_SIZE];
    char password[MAX_PASSWORD_SIZE];
} account_t;

typedef struct {
    char name[MAX_NAME_SIZE];
    list_t* sub_categories;
    list_t* accounts;
} category_t;

account_t* account_init(const char*, const char*);

category_t* category_init(const char*);
void category_add_account(category_t*, account_t*);
void category_add_subcategory(category_t*, category_t*);
void category_print(category_t*, int, int);
void category_free(category_t*);

#endif