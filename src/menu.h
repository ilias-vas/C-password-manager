#ifndef MENU_H
#define MENU_H

#include "util/defines.h"
#include "application.h"

#define PROMPT_USER(prompt, error, fn) do {     \
    int result = 1;                             \
    do {                                        \
        if (!result) printf("%s", error);       \
        printf("%s", prompt);                   \
    } while(!(result = fn));                    \
} while (0)

typedef struct menu_item {
    char name[MAX_NAME_SIZE];
    struct menu_item* prev;
    struct menu_item* next;
    void (*callback)(application_context_t*);
} menu_item_t;

typedef struct {
    char title[MAX_NAME_SIZE * 2];
    menu_item_t* first;
    menu_item_t* last;
    int count;
} menu_t;

menu_item_t* menu_item_init(const char*, void(*)(application_context_t*));

menu_item_t* menu_get_item(menu_t* menu, int i);
void menu_print(menu_t* menu);
void menu_push_item(menu_t* menu, menu_item_t* item);
void menu_free(menu_t* menu);

int get_string(char* result);
int get_int(int* result);
int get_int_range(int* result, int min, int max);

#endif
