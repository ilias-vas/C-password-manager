#include "menu.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

menu_item_t* new_menu_item(const char* name) {
    menu_item_t* ret = (menu_item_t*) malloc(sizeof(menu_item_t));
    if (ret) {
        strcpy(ret->name, name);
        ret->prev = NULL;
        ret->next = NULL;
    }
    return ret;
}

menu_item_t* get_menu_item(menu_t* menu, int i) {
    if (i >= menu->count) return NULL;
    
    menu_item_t* iter = menu->first;
    while(i-- && iter) iter = iter->next;

    return iter;
}

void print_menu(menu_t* menu) {
    menu_item_t* iter = menu->first;
    int i;
    for (i = 0; i < menu->count; ++i, iter = iter->next) {
        if (!iter) break;
        printf("%d. %s\n", i + 1, iter->name);
    }
}

void push_menu_item(menu_t* menu, menu_item_t* item) {
    if (menu->first == NULL) {
        menu->first = item;
        menu->last = item;
        menu->count = 1;
    } else {
        menu->last->next = item;
        item->prev = menu->last;
        menu->last = item;
        ++menu->count;
    }
}

void free_menu_items(menu_t* menu) {
    if (menu->count == 0) return;

    menu_item_t* iter = menu->first;
    while(iter) {
        menu_item_t* temp = iter;
        iter = iter->next;
        free(temp);
    }
    
    menu->first = NULL;
    menu->last = NULL;
    menu->count = 0;
}