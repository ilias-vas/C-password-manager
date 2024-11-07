#ifndef STREAM_H
#define STREAM_H

#include "defines.h"
#include "../account.h"

typedef struct {
   char* data;
   char* it;
   size_t size, capacity;
} stream_t;

stream_t stream_init(void);
void stream_free(stream_t*);

stream_t stream_from_path(const char* path, int* count);

int stream_save_to_file(stream_t* stream, const char* file_path);
int stream_read_from_file(stream_t* stream, const char* file_path);

void stream_push(stream_t* stream, const void* data, size_t size);
void stream_push_string(stream_t* stream, const char* string);
void stream_push_account(stream_t* stream, const account_t* account);
void stream_push_category(stream_t* stream, const category_t* category);

void* stream_pop(stream_t* stream, size_t size);
void stream_pop_string(stream_t* stream, char* result);
account_t* stream_pop_account(stream_t* stream);
category_t* stream_pop_category(stream_t* stream);

#endif
