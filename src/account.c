#include "account.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {};
}

void category_add_account(category_t* category, account_t* account) {
    list_append(category->accounts, account);
}

void category_remove_account(category_t* category, char *name) {
    clearInputBuffer();
    int i, j;
    char choice;
    int account_found = 0;

    /* loop through sub categories */
    for (i = 0; i < category->sub_categories->count; i++) {
        category_t* currCat = (category_t*) list_get(category->sub_categories, i);

        /* loop through every account in subcategories */
        for (j = 0; j < currCat->accounts->count; j++) {
            account_t* currAcc = (account_t*) list_get(currCat->accounts, j);

            /* if it's a match, remove */
            if (strcmp(currAcc->name, name) == 0) {
                printf(PMAN "Remove account %s? (y/n)\n", currAcc->name);
                printf("> ");

                scanf(" %c", &choice);
                if (choice == 'y') {
                    list_remove(currCat->accounts, j);
                    printf(PMAN "Account %s removed.\n", currAcc->name);
                    account_found = 1;
                    clearInputBuffer();
                    break;
                } else {
                    printf(PMAN "Account %s not removed\n", currAcc->name);
                    account_found = 1;
                    clearInputBuffer();
                }
            }
        }

        if (account_found) break;
    }

    if (!account_found) {
        printf("Account not found.\n");
    }
}

void category_add_subcategory(category_t* category, category_t* subcategory) {
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
