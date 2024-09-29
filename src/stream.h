#ifndef STREAM_H
#define STREAM_H

#include "defines.h"
#include "account.h"

typedef struct {
   char* data;
   char* it;
   size_t size, capacity;
} stream_t;

stream_t stream_init(void);
void stream_free(stream_t*);

int stream_save_to_file(stream_t*, const char*);
int stream_read_from_file(stream_t*, const char*);

void stream_push(stream_t*, const void*, size_t);
void stream_push_string(stream_t*, const char*);
void stream_push_account(stream_t*, const account_t*);
void stream_push_category(stream_t*, const category_t*);

void* stream_pop(stream_t*, size_t);
void stream_pop_string(stream_t*, char*);
account_t* stream_pop_account(stream_t*);
category_t* stream_pop_category(stream_t*);

#endif
