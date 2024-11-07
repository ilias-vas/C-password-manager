#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "util/list.h"
#include "util/defines.h"

typedef struct {
    char name[MAX_NAME_SIZE];
    char password[MAX_PASSWORD_SIZE];
} account_t;

typedef struct category{
    char name[MAX_NAME_SIZE];
    struct category* parent;
    list_t* sub_categories;
    list_t* accounts;
} category_t;

account_t* account_init(const char* name, const char* password);

category_t* category_init(const char* name);
category_t* category_find_category(const char* name, const category_t* parent);
account_t* category_find_account(const char* path, category_t* parent);
void category_add_account(category_t* category, account_t* account);
void category_remove_account(category_t* category, const char* path);
void category_add_subcategory(category_t* category, category_t* sub_category);
void category_print(category_t* category);
void category_free(category_t* category);

#endif
