#include <assert.h>
#include <stdlib.h>

#include "token.h"

Token *token_alloc(
        char *s,
        size_t len,
        Token_Type ty,
        size_t r,
        size_t c,
        const char *fp)
{
        Token *t = malloc(sizeof(Token));
        t->lx.s = s;
        t->lx.l = len;
        t->n = NULL;
        t->ty = ty;
        t->r = r;
        t->c = c;
        return t;
}

const char *token_type_to_cstr(Token_Type ty) {
        switch (ty) {
        case TOKEN_TYPE_UNKNOWN: return "TOKEN_TYPE_UNKNOWN";
        case TOKEN_TYPE_EOF: return "TOKEN_TYPE_EOF";
        case TOKEN_TYPE_NEWLINE: return "TOKEN_TYPE_NEWLINE";
        case TOKEN_TYPE_KEYWORD: return "TOKEN_TYPE_KEYWORD";
        case TOKEN_TYPE_IDENTIFIER: return "TOKEN_TYPE_IDENTIFIER";
        case TOKEN_TYPE_STRLIT: return "TOKEN_TYPE_STRLIT";
        case TOKEN_TYPE_INTLIT: return "TOKEN_TYPE_INTLIT";
        case TOKEN_TYPE_LPAREN: return "TOKEN_TYPE_LPAREN";
        case TOKEN_TYPE_RPAREN: return "TOKEN_TYPE_RPAREN";
        case TOKEN_TYPE_LCURLY_BRACKET: return "TOKEN_TYPE_LCURLY_BRACKET";
        case TOKEN_TYPE_RCURLY_BRACKET: return "TOKEN_TYPE_RCURLY_BRACKET";
        case TOKEN_TYPE_LSQR_BRACKET: return "TOKEN_TYPE_LSQR_BRACKET";
        case TOKEN_TYPE_RSQR_BRACKET: return "TOKEN_TYPE_RSQR_BRACKET";
        case TOKEN_TYPE_PLUS: return "TOKEN_TYPE_PLUS";
        case TOKEN_TYPE_MINUS: return "TOKEN_TYPE_MINUS";
        case TOKEN_TYPE_ASTERISK: return "TOKEN_TYPE_ASTERISK";
        case TOKEN_TYPE_FORWARD_SLASH: return "TOKEN_TYPE_FORWARD_SLASH";
        case TOKEN_TYPE_PERCENT: return "TOKEN_TYPE_PERCENT";
        case TOKEN_TYPE_EQUALS: return "TOKEN_TYPE_EQUALS";
        case TOKEN_TYPE_COMMA: return "TOKEN_TYPE_COMMA";
        case TOKEN_TYPE_SEMICOLON: return "TOKEN_TYPE_SEMICOLON";
        case TOKEN_TYPE_COLON: return "TOKEN_TYPE_COLON";
        case TOKEN_TYPE_DOLLAR: return "TOKEN_TYPE_DOLLAR";
        case TOKEN_TYPE_GREATER_THAN: return "TOKEN_TYPE_GREATER_THAN";
        case TOKEN_TYPE_LESS_THAN: return "TOKEN_TYPE_LESS_THAN";
        case TOKEN_TYPE_GREATER_THAN_EQUALS: return "TOKEN_TYPE_GREATER_THAN_EQUALS";
        case TOKEN_TYPE_LESS_THAN_EQUALS: return "TOKEN_TYPE_LESS_THAN_EQUALS";
        case TOKEN_TYPE_DOUBLE_EQUALS: return "TOKEN_TYPE_DOUBLE_EQUALS";
        case TOKEN_TYPE_DOUBLE_PIPE: return "TOKEN_TYPE_DOUBLE_PIPE";
        case TOKEN_TYPE_DOUBLE_AMPERSAND: return "TOKEN_TYPE_DOUBLE_AMPERSAND";
        default: assert(0 && "unknown token type");
        }
}
