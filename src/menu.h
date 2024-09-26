#ifndef MENU_H
#define MENU_H

#define MAX_NAME_SIZE 64

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
    void (*callback)(void);
} menu_item_t;

typedef struct {
    const char* title;
    menu_item_t* first;
    menu_item_t* last;
    int count;
} menu_t;

menu_item_t* new_menu_item(const char*, void(*)(void));
menu_item_t* get_menu_item(menu_t*, int);

void print_menu(menu_t*);
void push_menu_item(menu_t*, menu_item_t*);

int get_string(char*);
int get_int(int*);
int get_int_range(int*, int, int);

void free_menu_items(menu_t*);

#endif