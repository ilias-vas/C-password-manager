#include "list.h"

#include <stdlib.h>
#include <string.h>

void extend_list(list_t* list) {
    int capacity;
    if (list->capacity == 0) capacity = 1;
    else capacity = list->capacity << 2;
    void** data = (void**) malloc(sizeof(void*) * capacity);
    
    if (list->data) {
        memcpy(data, list->data, sizeof(void*) * list->capacity);
        free(list->data);
    }

    list->capacity = capacity;
    list->data = data;
}

list_t* make_list(void) {
    list_t* ret = (list_t*) malloc(sizeof(list_t));
    ret->count = 0;
    ret->capacity = 1;
    ret->data = (void**) malloc(sizeof(void*) * ret->capacity);
    return ret;
}


void list_append(list_t* list, void* data) {
    if (list->count == list->capacity) extend_list(list);
    list->data[list->count++] = data;
}

void list_remove(list_t* list, int i) {
    if (i < 0 || i >= list->count) return;
    if (list->count > 1) 
        memmove(list->data[i], list->data[i + 1], list->count - i);
    --list->count;
}

void* list_get(list_t* list, int i) {
    if (i < 0 || i >= list->count) return NULL;
    return list->data[i];
}

void list_empty(list_t* list) {
    list->count = 0;
}

void list_free(list_t* list) {
    free(list->data);
    list->capacity = 0;
    list->count = 0;
}
