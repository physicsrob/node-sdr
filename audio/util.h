#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <node.h>
#include <v8.h>

using namespace v8;

static inline char *TO_CHAR(Handle<Value> val) {
    String::Utf8Value utf8(val->ToString());

    int len = utf8.length() + 1;
    char *str = (char *) calloc(sizeof(char), len);
    strncpy(str, *utf8, len);

    return str;
}

#endif
