#ifndef LIST_H
#define LIST_H

typedef struct {
    void** data;
    int count, capacity;
} list_t;

list_t* make_list(void);
void list_append(list_t*, void*);
void list_remove(list_t*, int);
void* list_get(list_t*, int);

void list_empty(list_t*);
void list_free(list_t*);

#endif