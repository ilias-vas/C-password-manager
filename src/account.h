#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "util/list.h"
#include "util/defines.h"

typedef struct {
    char name[MAX_NAME_SIZE];
    char password[MAX_PASSWORD_SIZE];
} account_t;

typedef struct {
    char name[MAX_NAME_SIZE];
    list_t* sub_categories;
    list_t* accounts;
} category_t;

account_t* account_init(const char* name, const char* password);

category_t* category_init(const char* name);
void category_add_account(category_t* category, account_t* account);
void category_remove_account(category_t* category, char *name);
void category_add_subcategory(category_t* category, category_t* sub_category);
void category_print(category_t* category);
void category_free(category_t* category);

#endif
