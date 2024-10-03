#include <stdio.h>
#include <stdlib.h>

#include "account.h"
#include "application.h"
#include "util/defines.h"
#include "menu.h"
#include "vault.h"

void show_accounts_callback(application_context_t*);
void add_account_callback(application_context_t*);
void edit_account_callback(application_context_t*);
void remove_account_callback(application_context_t*);
void show_password_callback(application_context_t*);
void exit_callback(application_context_t*);

int main(void) {

    vault_t vault = {0};
    vault.file_path = "vault";

    if (vault_exists(&vault)) {
        printf(PMAN "Enter password.\n");
        do {
            PROMPT_USER("> ", REMOVE_LAST_LINE BOLDRED("Incorrect password. "), get_string(vault.password));
        } while(!vault_load(&vault));
    } else {
        printf(PMAN "No vault found, creating new vault.\n");
        printf(PMAN "Welcome. Please enter a new master password.\n");

        PROMPT_USER("> ", "", get_string(vault.password));
    }

    application_context_t app_context = {0};
    app_context.vault = &vault;

    menu_t main_menu = {0};
    sscanf(main_menu.title, PMAN "Welcome %s", vault.username);
    menu_push_item(&main_menu, menu_item_init("Show Accounts", &show_accounts_callback));
    menu_push_item(&main_menu, menu_item_init("Add Account", &add_account_callback));
    menu_push_item(&main_menu, menu_item_init("Edit Account", &edit_account_callback));
    menu_push_item(&main_menu, menu_item_init("Remove Account", &remove_account_callback));
    menu_push_item(&main_menu, menu_item_init("Show Password", &show_password_callback));
    menu_push_item(&main_menu, menu_item_init("Save and Exit", &exit_callback));

    while (1) {
        menu_print(&main_menu);
        int input;
        PROMPT_USER(
            "> ",
            REMOVE_LAST_LINE BOLDRED("Invalid Input. "),
            get_int_range(&input, 1, main_menu.count)
        );

        (*menu_get_item(&main_menu, input - 1)->callback)(&app_context);
    }

    menu_free(&main_menu);
    return 0;
}

void show_accounts_callback(application_context_t* context) {
    printf(PMAN "Accounts\n");
    category_print(context->vault->root);
}

void add_account_callback(application_context_t* context) {}

void edit_account_callback(application_context_t* context) {}

void remove_account_callback(application_context_t* context) {
    char accName[MAX_NAME_SIZE];
    printf(PMAN "Remove Acount\n");
    printf(PMAN "Enter Account name.\n");
    printf("> ");
    scanf("%s", accName);

    category_remove_account(context->vault->root, accName);
}

void show_password_callback(application_context_t* context) {}

void exit_callback(application_context_t* context) {
    vault_save(context->vault);
    exit(0);
}
