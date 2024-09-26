#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "defines.h"

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
    main_menu.title = PMAN " Welcome";
    menu_push_item(&main_menu, menu_item_init("Show Accounts", &show_accounts_callback));
    menu_push_item(&main_menu, menu_item_init("Add Account", &add_account_callback));
    menu_push_item(&main_menu, menu_item_init("Edit Account", &edit_account_callback));
    menu_push_item(&main_menu, menu_item_init("Remove Account", &remove_account_callback));
    menu_push_item(&main_menu, menu_item_init("Show Password", &show_password_callback));
    menu_push_item(&main_menu, menu_item_init("Exit", &exit_callback));
    
    while (1) {
        menu_print(&main_menu);
        int input;
        PROMPT_USER("> ", REMOVE_LAST_LINE BOLDRED("Invalid Input. "), get_int_range(&input, 1, 6));
        (*menu_get_item(&main_menu, input - 1)->callback)();
    }

    menu_free(&main_menu);
    return 0;
}

void show_accounts_callback(void) {}
void add_account_callback(void) {}
void edit_account_callback(void) {}
void remove_account_callback(void) {}
void show_password_callback(void) {}

void exit_callback(void) {
    exit(0);
}