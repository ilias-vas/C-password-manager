#include <stdio.h>

#include "menu.h"

int main() {
    
    menu_t main_menu = {0};
    
    push_menu_item(&main_menu, new_menu_item("this is the first entry"));
    push_menu_item(&main_menu, new_menu_item("this is the second entry"));
    push_menu_item(&main_menu, new_menu_item("this is the third entry"));
    
    print_menu(&main_menu);
    free_menu_items(&main_menu);


    return 0;
}
