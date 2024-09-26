#ifndef CATEGORY_H
#define CATEGORY_H

#include "account.h"

typedef struct category category_t;
typedef struct categoryList categoryList_t;

struct category {
    char name[MAX_NAME_LENGTH];
    int isCategory;
    account_t *accounts;
    categoryList *children;
};

struct categoryList {
    category_t *categories;
    int size;
    int capacity;
};

category_t *createCategory(const char *name);

#endif
