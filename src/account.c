#include "account.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util/stream.h"

#define TAB_WIDTH 2

account_t* account_init(const char* name, const char* password) {
    account_t* ret = (account_t*) malloc(sizeof(account_t));
    strcpy(ret->name, name);
    strcpy(ret->password, password);
    return ret;
}

category_t* category_init(const char* name) {
    category_t* ret = (category_t*) malloc(sizeof(category_t));
    strcpy(ret->name, name);
    ret->accounts = list_init();
    ret->sub_categories = list_init();
    return ret;
}

category_t* category_find_category(const char* name, const category_t* parent) {
    int i;
    for (i = 0; i < parent->sub_categories->count; ++i) {
        category_t* sub_category = list_get(parent->sub_categories, i);
        if (strcmp(name, sub_category->name) == 0) return sub_category;
    }
    return NULL;
}

/* get account based on a path eg: cat1/cat2/acc_name */
account_t* category_find_account(const char* path, category_t* parent) {
    int i, count;
    char name[MAX_NAME_SIZE];
    stream_t names = stream_from_path(path, &count);

    category_t* current = parent;
    for (i = 0; i < count - 1; ++i) {
        stream_pop_string(&names, name);
        category_t* next = category_find_category(name, current);
        if (next) current = next;
        else {
            stream_free(&names);
            return NULL;
        }
    }

    account_t* account;
    stream_pop_string(&names, name);
    for (i = 0; i < current->accounts->count; ++i) {
        account = list_get(current->accounts, i);
        if (strcmp(account->name, name) == 0) {
            stream_free(&names);
            return account;
        }
    }

    stream_free(&names);
    return NULL;
}

void category_add_account(category_t* category, account_t* account) {
    list_append(category->accounts, account);
}

void category_remove_subcategory(category_t* parent, category_t* subcategory) {
    int i;
    for (i = 0; i < parent->sub_categories->count; ++i) {
        category_t* current = list_get(parent->sub_categories, i);
        if (current != subcategory) continue;
        list_remove(parent->sub_categories, i);
        return;
    }
}

void category_remove_account(category_t* parent, const char* path) {
    int i, count;
    char name[MAX_NAME_SIZE];
    stream_t names = stream_from_path(path, &count);

    category_t* current = parent;
    for (i = 0; i < count - 1; ++i) {
        stream_pop_string(&names, name);
        category_t* next = category_find_category(name, current);
        if (next) current = next;
        else {
            stream_free(&names);
            return;
        }
    }

    account_t* account;
    stream_pop_string(&names, name);
    for (i = 0; i < current->accounts->count; ++i) {
        account = list_get(current->accounts, i);
        if (strcmp(account->name, name) == 0) {
            list_remove(current->accounts, i);
            break;
        }
    }

    while(current->accounts->count == 0 && current->sub_categories->count == 0) {
        category_t* old = current;
        current = current->parent;
        category_remove_subcategory(current, old);
    }

    stream_free(&names);
}

void category_add_subcategory(category_t* category, category_t* subcategory) {
    subcategory->parent = category;
    list_append(category->sub_categories, subcategory);
}

void print_tab(int level, int is_last) {
    if (level == 0) return;

    printf("%*s", TAB_WIDTH, "");
    --level;

    const char* delim = (is_last) ? " " : "│";
    while(level--) printf(PRIMARY_COLOUR("%s") "%*s", delim, TAB_WIDTH, "");
}

void _category_print(category_t* category, int level, int is_last) {
    printf(BOLD_SECONDARY_COLOUR("%s\n"), category->name);

    int i;
    for (i = 0; i < category->sub_categories->count; i++) {
        int is_last =
            i == category->sub_categories->count - 1 &&
            category->accounts->count == 0;
        const char* delim = is_last ? "└─" : "├─";

        print_tab(level + 1, is_last);
        printf(PRIMARY_COLOUR("%s"), delim);
        _category_print((category_t*) list_get(category->sub_categories, i), level + 1, is_last);
    }

    for (i = 0; i < category->accounts->count; i++) {
        account_t* account = (account_t*) list_get(category->accounts, i);
        const char* delim = (i == category->accounts->count - 1) ? "└─" : "├─";

        print_tab(level + 1, is_last);
        printf(PRIMARY_COLOUR("%s") "%s\n", delim, account->name);
    }
}

void category_print(category_t* category) {
    _category_print(category, 0, 0);
}

void category_free(category_t* category) {
    int i;
    for (i = 0; i < category->accounts->count; i++) {
        account_t* account = (account_t*) list_get(category->accounts, i);
        free(account);
    }

    for (i = 0; i < category->sub_categories->count; i++) {
        category_t* subcategory = (category_t*) list_get(category->sub_categories, i);
        category_free(subcategory);
    }
    free(category);
}
