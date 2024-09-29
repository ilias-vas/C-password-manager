#include "stream.h"
#include "list.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define INITIAL_CAPACITY 64

stream_t stream_init(void) {
    stream_t ret = {0};
    ret.size = 0;
    ret.capacity = INITIAL_CAPACITY;
    ret.data = (char*) malloc(sizeof(char) * ret.capacity);
    ret.it = ret.data;
    return ret;
}

void stream_free(stream_t* stream) {
    stream->size = 0;
    stream->capacity = INITIAL_CAPACITY;
    free(stream->data);
    stream->data = NULL;
    stream->it = stream->data;
}

int stream_save_to_file(stream_t* stream, const char* path) {
    FILE* file = fopen(path, "wb");
    if (file == NULL) return 0;

    fwrite(stream->data, sizeof(char), stream->size, file);

    fclose(file);
    return 1;
}

int stream_read_from_file(stream_t* stream, const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) return 0;

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    free(stream->data);
    stream->size = 0;
    stream->capacity = size;
    stream->data = (char*) malloc(sizeof(char) * stream->capacity);
    stream->it = stream->data;

    fread(stream->data, sizeof(char), size, file);
    fclose(file);
    return 1;
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

void* stream_pop(stream_t* stream, size_t size) {
    if (stream->it - stream->data + size > stream->capacity) return NULL;
    stream->it += size;
    return stream->it - size;
}

void stream_pop_string(stream_t* stream, char* data) {
    strcpy(data, stream->it);
    stream->it += strlen(data) + 1;
}

account_t* stream_pop_account(stream_t* stream) {
    char name[MAX_NAME_SIZE];
    stream_pop_string(stream, name);
    char password[MAX_PASSWORD_SIZE];
    stream_pop_string(stream, password);
    return account_init(name, password);
}

category_t* stream_pop_category(stream_t* stream) {
    char name[MAX_NAME_SIZE];
    stream_pop_string(stream, name);
    category_t* category = category_init(name);

    int i;
    for (i = *(int*) stream_pop(stream, sizeof(int)); i > 0; --i) {
        list_append(category->accounts, stream_pop_account(stream));
    }

    for (i = *(int*) stream_pop(stream, sizeof(int)); i > 0; --i)
        list_append(category->sub_categories, stream_pop_category(stream));

    return category;
}
