#include <stddef.h>

#include <pcre2.h>

#include "str.h"
#include "lexer.h"
#include "utils.h"

int
set_lexeme(pcre2_callout_block * callout_block, void * data) {
    enum LexemeType * lexeme_type = (enum LexemeType *)data;
    *lexeme_type = callout_block->callout_number;
    return 0;
}

struct Parser *
parser_new(struct Str * input) {
    pcre2_code * regex;
    pcre2_match_data * match_data;
    pcre2_match_context * match_context;
    int error_number;
    PCRE2_SIZE erroroffset;
    struct Str * pattern;

    pattern = str_format(
            "^("
#define DEF_LEXEME(name, pattern) pattern "(?C%d)" "|"
#define LEXEME_EXAMPLE(name, example)
#include "lexemes.def"
            "$" "(?C%d)"
            ")" DEFINITIONS,
#define DEF_LEXEME(name, pattern) name,
#define LEXEME_EXAMPLE(name, example)
#include "lexemes.def"
            LEX_NULL
            );
    static _Bool first_call = 1;
    if (first_call) {
        log_msg(LL_DEBUG, "pattern = \"%s\"", pattern->str);
        first_call = 0;
    }
    regex = pcre2_compile(
        (uint8_t *)pattern->str,        /* the pattern */
        PCRE2_ZERO_TERMINATED,          /* indicates pattern is zero‐terminated */
        PCRE2_UTF | PCRE2_MULTILINE,    /* default options */
        &error_number,                  /* for error number */
        &erroroffset,                   /* for error offset */
        NULL);                          /* use default compile context */
    if (!regex) {
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(error_number, buffer, sizeof(buffer));
        log_msg(LL_ERROR, "PCRE2 compilation failed at offset %d: %s", (int)erroroffset, buffer);
        log_msg(LL_ERROR, "pattern: %.50s\n", &pattern->str[erroroffset]);
        exit(1);
    }
    str_delete(pattern);
    match_data = pcre2_match_data_create_from_pattern(regex, NULL);
    if (!match_data) {
        log_msg(LL_ERROR, "Failed to create match data.");
        exit(1);
    }
    match_context = pcre2_match_context_create(NULL);
    if (!match_context) {
        log_msg(LL_ERROR, "Failed to create match context.");
        exit(1);
    }
    struct Parser * parser = mem_alloc(sizeof(struct Parser));
    *parser = (struct Parser) {
        .regex               = regex,
        .match_data          = match_data,
        .match_context       = match_context,
        .location            = (struct Location) {
            .line   = 1,
            .column = 1,
            .offset = 0,
        },
        .input               = input,
        .current_lexeme      = LEX_NULL,
        .current_lexeme_str  = NULL
    };
    error_number = pcre2_set_callout(match_context, set_lexeme, &parser->current_lexeme);
    if (error_number < 0) {
        log_msg(LL_ERROR, "Failed to set callout.");
        exit(1);
    }
    return parser;
}

void
parser_delete(struct Parser * parser) {
    pcre2_code_free(parser->regex);
    pcre2_match_data_free(parser->match_data);
    mem_free(parser);
}

void
parser_next_lexeme(struct Parser * parser) {
    int error;

    error = pcre2_match(
        parser->regex,                  /* the compiled pattern */
        (uint8_t *)parser->input->str,  /* the subject string */
        parser->input->length,          /* the length of the subject */
        0,                              /* start at offset 0 in the subject */
        0,                              /* default options */
        parser->match_data,             /* block for storing the result */
        parser->match_context);         /* use default match context */
    if (error < 0) {
//        PCRE2_UCHAR buffer[256];
//        pcre2_get_error_message(error, buffer, sizeof(buffer));
//        log_msg(LL_ERROR, "Unknown lexeme at %d: %s", (int)parser->location.offset, buffer);
//        exit(1);
    }
}
