#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "utils.h"
#include "str.h"

struct Str *
str_new(size_t length) {
    struct Str * str = mem_alloc(sizeof(struct Str) + length + 1);
    str->length = length;
    return str;
}

void
str_delete(struct Str * str) {
    mem_free(str);
}

struct Str *
str_from_c(const char * str) {
    size_t length = strlen(str);
    struct Str * p = str_new(length);
    p->length = length;
    strcpy(p->str, str);
    return p;
}

struct Str *
str_from_c_n(const char * cstr, size_t length) {
    struct Str * str = str_new(length);
    str->length = length;
    *strncpy(str->str, cstr, length) = '\0';
    return str;
}

struct Str *
str_format(const char * format, ...) {
    va_list ap;
    va_start(ap, format);
    int length = vsnprintf(NULL, 0, format, ap);
    va_end(ap);
    struct Str * str = str_new(length);
    va_start(ap, format);
    vsnprintf(str->str, length + 1, format, ap);
    va_end(ap);
    return str;
}
