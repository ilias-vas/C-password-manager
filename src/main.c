#include <stdio.h>
#include <stdlib.h>

#include "account.h"
#include "application.h"
#include "util/defines.h"
#include "util/stream.h"
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
        vault.root = category_init("root");
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
    category_free(vault.root);
    return 0;
}

void show_accounts_callback(application_context_t* context) {
    printf(PMAN "Accounts\n");
    category_print(context->vault->root);
}

void add_account_callback(application_context_t* context) {
    puts(PMAN "Add Account");
    char path[MAX_PATH_SIZE];
    char password[MAX_PASSWORD_SIZE];

    puts("Enter account path. eg accountName or categoryName/subcategoryName/accountName");
    PROMPT_USER( "> ", "", get_string(path));

    if (category_find_account(path, context->vault->root)) {
        puts(RED("Account already exists"));
        return;
    }

    puts("Enter password for account");
    PROMPT_USER( "> ", "", get_string(password));

    char name[MAX_NAME_SIZE];
    int i, count;
    category_t* parent = context->vault->root;
    stream_t names = stream_from_path(path, &count);
    for (i = 0; i < count - 1; ++i) {
        stream_pop_string(&names, name);

        category_t* category;
        category = category_find_category(name, parent);
        if (!category) {
            category = category_init(name);
            category_add_subcategory(parent, category);
        }

        parent = category;   
    }

    stream_pop_string(&names, name);
    account_t* account = account_init(name, password);
    category_add_account(parent, account);
    puts(GREEN("Account added successfully"));
}

void edit_account_callback(application_context_t* context) {}

void remove_account_callback(application_context_t* context) {
    
}

void show_password_callback(application_context_t* context) {}

void exit_callback(application_context_t* context) {
    vault_save(context->vault);
    exit(0);
}
