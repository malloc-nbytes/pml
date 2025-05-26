#ifndef TOKEN_HXX
#define TOKEN_HXX

#include <stddef.h>

namespace token {
    typedef enum {
        unknown,
        eof,
        newline,
        strlit,
        intlit,
        keyword,
        identifier,
        lsqr_bracket,
        rsqr_bracket,
        lcurly_bracket,
        rcurly_bracket,
        lparen,
        rparen,
        dollar,
        semicolon,
        comma,
        plus,
        minus,
        asterisk,
        forward_slash,
        greater_than,
        less_than,
        equals,
        percent,
        period,
        greater_than_equals,
        less_than_equals,
        double_equals,
        percent_equals,
        double_pipe,
        double_ampersand,
    } type_t;

    typedef struct token {
        struct {
            char *st;
            size_t l;
        } lx;
        type_t ty;
        size_t r, c;
        token *n;
    } token_t;

    token_t *create(char *st, size_t l,
                    type_t ty, size_t r,
                    size_t c);
};

#endif // TOKEN_HXX
