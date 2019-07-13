#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "utils.h"

void *
mem_alloc(size_t n) {
    void * p = malloc(n);
    if (!p) {
        log_msg(LL_ERROR, "Out of memory");
        exit(1);
    }
    return p;
}

void
mem_free(void * p) {
    free(p);
}

void
log_msg(enum LogLevel log_level, const char * format, ...) {
    va_list ap;
    const char * log_level_str;
    switch (log_level) {
        case LL_ERROR:   log_level_str = "[\033[0;31mERROR  \033[0;0m] "; break;
        case LL_WARNING: log_level_str = "[\033[0;33mWARNING\033[0;0m] "; break;
        case LL_INFO:    log_level_str = "[\033[0;34mINFO   \033[0;0m] "; break;
        case LL_DEBUG:   log_level_str = "[\033[0;36mDEBUG  \033[0;0m] "; break;
        default:         log_level_str = "[\033[0;35mUNKNOWN\033[0;0m] "; break;
    }
    va_start(ap, format);
    fputs(log_level_str, stderr);
    vfprintf(stderr, format, ap);
    fputs("\n", stderr);
    va_end(ap);
}
