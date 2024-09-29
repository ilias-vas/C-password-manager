#ifndef STREAM_H
#define STREAM_H

#include "defines.h"
#include "account.h"

typedef struct {
   char* data;
   size_t size, capacity;
} stream_t;

stream_t stream_init(void);

void stream_push(stream_t*, const void*, size_t);
void stream_push_string(stream_t*, const char*);
void stream_push_account(stream_t*, const account_t*);
void stream_push_category(stream_t*, const category_t*);


#endif
