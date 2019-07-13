enum LogLevel {
    LL_ERROR,
    LL_WARNING,
    LL_INFO,
    LL_DEBUG,
};

void * mem_alloc(size_t n);
void mem_free(void * p);
void log_msg(enum LogLevel log_level, const char * format, ...);
