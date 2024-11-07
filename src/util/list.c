#include "list.h"

#include <stdlib.h>
#include <string.h>

void list_extend(list_t* list) {
    if (list->capacity < 1) list->capacity = 1;
    list->capacity *= 2;
    if (!list->data) list->data = (void**) malloc(sizeof(void*) * list->capacity);
    else list->data = (void**) realloc(list->data, sizeof(void*) * list->capacity);
}

list_t* list_init(void) {
    list_t* ret = (list_t*) malloc(sizeof(list_t));
    ret->count = 0;
    ret->capacity = 1;
    ret->data = (void**) malloc(sizeof(void*) * ret->capacity);
    return ret;
}

list_t* list_copy(list_t* other) {
    list_t* ret = (list_t*) malloc(sizeof(list_t));
    ret->count = other->count;
    ret->capacity = other->capacity;
    ret->data = (void**) malloc(sizeof(void*) * ret->capacity);
    memcpy(ret->data, other->data, ret->capacity);
    return ret;
}

void list_append(list_t* list, void* data) {
    if (list->count == list->capacity) list_extend(list);
    list->data[list->count++] = data;
}

void list_remove(list_t* list, int i) {
    if (i < 0 || i >= list->count) return;
    if (list->count > 1)
        memmove(list->data + i, list->data + i + 1, list->count - i);
    --list->count;
}

void* list_get(list_t* list, int i) {
    if (i < 0 || i >= list->count) return NULL;
    return list->data[i];
}

void list_set(list_t* list, int i, void* data) {
    if (i < 0 || i >= list->count) return;
    list->data[i] = data;
}

void list_empty(list_t* list) {
    list->count = 0;
}

void list_free(list_t* list) {
    free(list->data);
    free(list);
}
