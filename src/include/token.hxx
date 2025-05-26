#ifndef TOKEN_HXX
#define TOKEN_HXX

#include <stddef.h>

typedef enum {
    TT_UNKNOWN,
    TT_EOF,
    TT_NEWLINE,
    TT_STRLIT,
    TT_INTLIT,
    TT_KEYWORD,
    TT_IDENTIFIER,
    TT_LSQR_BRACKET,
    TT_RSQR_BRACKET,
    TT_LCURLY_BRACKET,
    TT_RCURLY_BRACKET,
    TT_LPAREN,
    TT_RPAREN,
    TT_DOLLAR,
    TT_SEMICOLON,
    TT_COMMA,
    TT_PLUS,
    TT_MINUS,
    TT_ASTERISK,
    TT_FORWARD_SLASH,
    TT_GREATER_THAN,
    TT_LESS_THAN,
    TT_EQUALS,
    TT_PERCENT,
    TT_PERIOD,
    TT_GREATER_THAN_EQUALS,
    TT_LESS_THAN_EQUALS,
    TT_DOUBLE_EQUALS,
    TT_PERCENT_EQUALS,
    TT_DOUBLE_PIPE,
    TT_DOUBLE_AMPERSAND,
} token_type_t;

const char *token_type_to_cstr(token_type_t ty);

typedef struct token {
    struct {
        char *st;
        size_t l;
    } lx;
    token_type_t ty;
    size_t r, c;
    token *n;
} token_t;

token_t *token_create(char *st, size_t l,
                      token_type_t ty, size_t r,
                      size_t c);

#endif // TOKEN_HXX
