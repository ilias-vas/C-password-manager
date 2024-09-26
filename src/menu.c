#include "menu.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "escapes.h"

#define BUFFER_SIZE 64

menu_item_t* menu_item_init(const char* name, void (*callback) (void)) {
    menu_item_t* ret = (menu_item_t*) malloc(sizeof(menu_item_t));
    if (ret) {
        strcpy(ret->name, name);
        ret->callback = callback;
        ret->prev = NULL;
        ret->next = NULL;
    }
    return ret;
}

menu_item_t* menu_get_item(menu_t* menu, int i) {
    if (i < 0 || i >= menu->count) return NULL;
    
    menu_item_t* iter = menu->first;
    while(i-- && iter) iter = iter->next;

    return iter;
}

void menu_print(menu_t* menu) {
    menu_item_t* iter = menu->first;
    printf("%s\n", menu->title);
    int i;
    for (i = 0; i < menu->count; ++i, iter = iter->next) {
        if (!iter) break;
        printf( CYAN("[%d] ") "%s\n", i + 1, iter->name);
    }
}

void menu_push_item(menu_t* menu, menu_item_t* item) {
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

void menu_free(menu_t* menu) {
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

int get_string(char* data) {
    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, stdin);

    /* we dont read directly into data as it would include the new line*/
    sscanf(buffer, "%s", data);
    return 1;
}

int get_int(int* data) {
    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, stdin);

    if (sscanf(buffer, "%d", data) != 1) return 0;
    return 1;
}

int get_int_range(int* data, int min, int max) {
    if (!get_int(data)) return 0;
    return (*data >= min && *data <= max);
}