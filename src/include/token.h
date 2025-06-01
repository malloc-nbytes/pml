#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

typedef enum {
        TOKEN_TYPE_UNKNOWN,
        TOKEN_TYPE_EOF,
        TOKEN_TYPE_NEWLINE,
        TOKEN_TYPE_KEYWORD,
        TOKEN_TYPE_IDENTIFIER,
        TOKEN_TYPE_STRLIT,
        TOKEN_TYPE_INTLIT,

        TOKEN_TYPE_LPAREN,
        TOKEN_TYPE_RPAREN,
        TOKEN_TYPE_LCURLY_BRACKET,
        TOKEN_TYPE_RCURLY_BRACKET,
        TOKEN_TYPE_LSQR_BRACKET,
        TOKEN_TYPE_RSQR_BRACKET,
        TOKEN_TYPE_PLUS,
        TOKEN_TYPE_MINUS,
        TOKEN_TYPE_ASTERISK,
        TOKEN_TYPE_FORWARD_SLASH,
        TOKEN_TYPE_PERCENT,
        TOKEN_TYPE_EQUALS,
        TOKEN_TYPE_COMMA,
        TOKEN_TYPE_SEMICOLON,
        TOKEN_TYPE_COLON,
        TOKEN_TYPE_DOLLAR,
        TOKEN_TYPE_GREATER_THAN,
        TOKEN_TYPE_LESS_THAN,
        TOKEN_TYPE_GREATER_THAN_EQUALS,
        TOKEN_TYPE_LESS_THAN_EQUALS,
        TOKEN_TYPE_DOUBLE_EQUALS,
        TOKEN_TYPE_DOUBLE_PIPE,
        TOKEN_TYPE_DOUBLE_AMPERSAND,
        TOKEN_TYPE_BANG_EQUALS,
        TOKEN_TYPE_BANG,
        TOKEN_TYPE_DOUBLE_SEMICOLON,
} Token_Type;

typedef struct Token {
        struct {
                char *s;
                size_t l;
        } lx;
        Token_Type ty;
        struct Token *n;
        size_t r;
        size_t c;
        const char *fp;
} Token;

Token *token_alloc(char *s, size_t len, Token_Type ty, size_t r, size_t c, const char *fp);
const char *token_type_to_cstr(Token_Type ty);

#endif // TOKEN_H
