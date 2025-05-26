#include <cassert>

#include "token.hxx"

token_t *token_create(char *st, size_t l,
                      token_type_t ty, size_t r,
                      size_t c) {
    auto t = new token_t;
    t->n = nullptr;
    t->lx.st = st;
    t->lx.l = l;
    t->ty = ty;
    t->r = r;
    t->c = c;
    return t;
}

const char *token_type_to_cstr(token_type_t ty) {
    switch (ty) {
    case TT_UNKNOWN: return "unknown";
    case TT_EOF: return "eof";
    case TT_NEWLINE: return "newline";
    case TT_STRLIT: return "strlit";
    case TT_INTLIT: return "intlit";
    case TT_KEYWORD: return "keyword";
    case TT_IDENTIFIER: return "identifier";
    case TT_LSQR_BRACKET: return "lsqr_bracket";
    case TT_RSQR_BRACKET: return "rsqr_bracket";
    case TT_LCURLY_BRACKET: return "lcurly_bracket";
    case TT_RCURLY_BRACKET: return "rcurly_bracket";
    case TT_LPAREN: return "lparen";
    case TT_RPAREN: return "rparen";
    case TT_DOLLAR: return "dollar";
    case TT_SEMICOLON: return "semicolon";
    case TT_COMMA: return "comma";
    case TT_PLUS: return "plus";
    case TT_MINUS: return "minus";
    case TT_ASTERISK: return "asterisk";
    case TT_FORWARD_SLASH: return "forward_slash";
    case TT_GREATER_THAN: return "greater_than";
    case TT_LESS_THAN: return "less_than";
    case TT_EQUALS: return "equals";
    case TT_PERCENT: return "percent";
    case TT_PERIOD: return "period";
    case TT_GREATER_THAN_EQUALS: return "greater_than_equals";
    case TT_LESS_THAN_EQUALS: return "less_than_equals";
    case TT_DOUBLE_EQUALS: return "double_equals";
    case TT_PERCENT_EQUALS: return "percent_equals";
    case TT_DOUBLE_PIPE: return "double_pipe";
    case TT_DOUBLE_AMPERSAND: return "double_ampersand";
    default: assert(0 && "unknown token");
    }
}
