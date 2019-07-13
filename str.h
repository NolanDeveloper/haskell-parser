struct Str {
    size_t length;
    char str[];
};

struct Str * str_new(size_t length);
void str_delete(struct Str * p);
struct Str * str_from_c(const char * str);
struct Str * str_from_c_n(const char * str, size_t length);
struct Str * str_format(const char * format, ...);
