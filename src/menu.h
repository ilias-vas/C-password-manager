#ifndef MENU_H
#define MENU_H

#define MAX_NAME_SIZE 64

typedef struct menu_item {
    char name[MAX_NAME_SIZE];
    struct menu_item* prev;
    struct menu_item* next;
} menu_item_t;

typedef struct {
    menu_item_t* first;
    menu_item_t* last;
    int count;
} menu_t;

menu_item_t* new_menu_item(const char*);
menu_item_t* get_menu_item(menu_t*, int);

void print_menu(menu_t*);
void push_menu_item(menu_t*, menu_item_t*);

void free_menu_items(menu_t*);

#endif