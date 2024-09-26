#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "defines.h"
#include "account.h"

void show_accounts_callback(void);
void add_account_callback(void);
void edit_account_callback(void);
void remove_account_callback(void);
void show_password_callback(void);
void exit_callback(void);

int main() {
    
    /* if there is no stored database and create a new user */

    /* 
    char* password;
    PROMPT_USER(PMAN "Enter password.\n>", "", get_string(password));
    */
    
    /* check if the password is correct here */

    /* load stored information about the user */

    menu_t main_menu = {0};
    main_menu.title = PMAN "Welcome";
    menu_push_item(&main_menu, menu_item_init("Show Accounts", &show_accounts_callback));
    menu_push_item(&main_menu, menu_item_init("Add Account", &add_account_callback));
    menu_push_item(&main_menu, menu_item_init("Edit Account", &edit_account_callback));
    menu_push_item(&main_menu, menu_item_init("Remove Account", &remove_account_callback));
    menu_push_item(&main_menu, menu_item_init("Show Password", &show_password_callback));
    menu_push_item(&main_menu, menu_item_init("Exit", &exit_callback));
    
    while (1) {
        menu_print(&main_menu);
        int input;
        PROMPT_USER(
            "> ",
            REMOVE_LAST_LINE BOLDRED("Invalid Input. "),
            get_int_range(&input, 1, main_menu.count)
        );

        (*menu_get_item(&main_menu, input - 1)->callback)();
    }

    menu_free(&main_menu);
    return 0;
}

void show_accounts_callback(void) {
    printf(PMAN "Accounts\n");
    /* this is just for testing */
    category_t* root = category_init("root");

    category_t* important = category_init("important");
    category_t* gmail = category_init("gmail");
    category_add_account(gmail, account_init("personal", "test"));
    category_add_account(gmail, account_init("work", "test"));
    category_add_account(gmail, account_init("school", "test"));

    category_add_subcategory(important, gmail);
    category_add_account(important, account_init("bank", "test"));
    category_add_subcategory(root, important);

    category_t* test = category_init("test");
    category_add_subcategory(root, test);

    category_t* social = category_init("social");
    category_add_account(social, account_init("instagram", "test"));
    category_add_account(social, account_init("reddit", "test"));
    category_add_account(social, account_init("github", "test"));
    category_add_subcategory(root, social);

    category_print(root, 0, 0);

    category_free(root);
}

void add_account_callback(void) {}

void edit_account_callback(void) {}

void remove_account_callback(void) {}

void show_password_callback(void) {}

void exit_callback(void) {
    exit(0);
}
