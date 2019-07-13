enum LexemeType {
    LEX_NULL,
#define DEF_LEXEME(name, pattern) name,
#define LEXEME_EXAMPLE(name, example)
#include "lexemes.def"
};

struct Location {
    size_t offset;  /* offset in buffer */
    size_t line;    /* line number starting from 1 */
    size_t column;  /* column number in the specified line starting from 1 */
};

struct Lexeme {
    enum LexemeType type;
    struct Str * symbols;
    struct Location location;
};

struct Parser {
    pcre2_code * regex;
    pcre2_match_data * match_data;
    pcre2_match_context * match_context;
    struct Location location;
    struct Str * input;
    enum LexemeType current_lexeme;
    struct Str * current_lexeme_str;
};

struct Parser * parser_new(struct Str * input);
void parser_delete(struct Parser * parser);
void parser_next_lexeme(struct Parser * parser);
