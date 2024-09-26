#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "escapes.h"

#define PMAN CYAN("[pman]")

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
    push_menu_item(&main_menu, new_menu_item("Show Accounts", &show_accounts_callback));
    push_menu_item(&main_menu, new_menu_item("Add Account", &add_account_callback));
    push_menu_item(&main_menu, new_menu_item("Edit Account", &edit_account_callback));
    push_menu_item(&main_menu, new_menu_item("Remove Account", &remove_account_callback));
    push_menu_item(&main_menu, new_menu_item("Show Password", &show_password_callback));
    push_menu_item(&main_menu, new_menu_item("Exit", &exit_callback));
    
    while (1) {
        print_menu(&main_menu);
        int input;
        PROMPT_USER("> ", REMOVE_LAST_LINE BOLDRED("Invalid Input. "), get_int_range(&input, 1, 6));
        (*get_menu_item(&main_menu, input - 1)->callback)();
    }

    free_menu_items(&main_menu);
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
