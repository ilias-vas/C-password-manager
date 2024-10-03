#ifndef LIST_H
#define LIST_H

typedef struct {
    void** data;
    int count, capacity;
} list_t;

list_t* list_init(void);
list_t* list_copy(list_t* list);
void list_append(list_t* list, void* data);
void list_remove(list_t* list, int i);
void* list_get(list_t* list, int i);
void list_set(list_t* list, int i, void* data);

void list_empty(list_t* list);
void list_free(list_t* list);

#endif
