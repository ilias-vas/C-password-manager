#include "stream.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define INITIAL_CAPACITY 64

stream_t stream_init(void) {
    stream_t ret = {0};
    ret.size = 0;
    ret.capacity = INITIAL_CAPACITY;
    ret.data = (char*) malloc(sizeof(char) * ret.capacity);
    return ret;
}

void stream_reserve(stream_t* stream, size_t size) {
    if (stream->capacity < INITIAL_CAPACITY) stream->capacity = INITIAL_CAPACITY;
    while (stream->size + size > stream->capacity) stream->capacity *= 2;
    if (!stream->data) stream->data = (char*) malloc(stream->capacity);
    else stream->data = (char*) realloc(stream->data, stream->capacity);
}

void stream_push(stream_t* stream, const void* data, size_t size) {
    stream_reserve(stream, size);
    memcpy(stream->data + stream->size, (char*) data, size);
    stream->size += size;
}

void stream_push_string(stream_t* stream, const char* string) {
    /* add one to make sure we include the null byte */
    stream_push(stream, string, strlen(string) + 1);
}

void stream_push_account(stream_t* stream, const account_t* account) {
    stream_push_string(stream, account->name);
    stream_push_string(stream, account->password);
}

void stream_push_category(stream_t* stream, const category_t* category) {
    stream_push_string(stream, category->name);

    int i;
    stream_push(stream, &category->accounts->count, sizeof(int));
    for (i = 0; i < category->accounts->count; ++i)
        stream_push_account(stream, list_get(category->accounts, i));

    stream_push(stream, &category->sub_categories->count, sizeof(int));
    for (i = 0; i < category->sub_categories->count; ++i)
        stream_push_category(stream, list_get(category->sub_categories, i));
}

