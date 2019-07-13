#include <stdio.h>

#include <pcre2.h>

#include "str.h"
#include "lexer.h"
#include "utils.h"

#define ASSERT(condition, comment) my_assert(condition, comment)

void
my_assert(_Bool condition, const char * comment) {
    if (condition) {
        log_msg(LL_INFO, "\033[0;32mOK  \033[0m: %s", comment);
    } else {
        log_msg(LL_INFO, "\033[0;31mFAIL\033[0m: %s", comment);
    }
}

void
start_test(const char * name) {
    log_msg(LL_INFO, "########## TEST \"%s\" ##########", name);
}

void
test_lexer(void) {
    struct {
        enum LexemeType expected_lexeme;
        const char * input;
        const char * comment;
    } data[] = {
#define DEF_LEXEME(name, pattern)
#define LEXEME_EXAMPLE(lexeme, input) { lexeme, input, #lexeme " <- " #input },
#include "lexemes.def"
    };
    start_test("Lexer");
    for (size_t i = 0; i < sizeof(data) / sizeof(data[0]); ++i) {
        enum LexemeType expected_lexeme = data[i].expected_lexeme;
        struct Str * input = str_from_c(data[i].input);
        struct Parser * parser = parser_new(input);
        parser_next_lexeme(parser);
        struct Str * comment = str_format("%s: %d", data[i].comment, parser->current_lexeme);
        ASSERT(parser->current_lexeme == expected_lexeme, comment->str);
        str_delete(comment);
        parser_delete(parser);
        str_delete(input);
    }
}

int
main() {
    test_lexer();
}
