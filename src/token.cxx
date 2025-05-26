#include <cassert>

#include "token.hxx"

using namespace token;

token_t *token::create(char *st, size_t l,
                       type_t ty, size_t r,
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

const char *token::type_to_cstr(type_t ty) {
    switch (ty) {
    case type_t::unknown: return "unknown";
    case type_t::eof: return "eof";
    case type_t::newline: return "newline";
    case type_t::strlit: return "strlit";
    case type_t::intlit: return "intlit";
    case type_t::keyword: return "keyword";
    case type_t::identifier: return "identifier";
    case type_t::lsqr_bracket: return "lsqr_bracket";
    case type_t::rsqr_bracket: return "rsqr_bracket";
    case type_t::lcurly_bracket: return "lcurly_bracket";
    case type_t::rcurly_bracket: return "rcurly_bracket";
    case type_t::lparen: return "lparen";
    case type_t::rparen: return "rparen";
    case type_t::dollar: return "dollar";
    case type_t::semicolon: return "semicolon";
    case type_t::comma: return "comma";
    case type_t::plus: return "plus";
    case type_t::minus: return "minus";
    case type_t::asterisk: return "asterisk";
    case type_t::forward_slash: return "forward_slash";
    case type_t::greater_than: return "greater_than";
    case type_t::less_than: return "less_than";
    case type_t::equals: return "equals";
    case type_t::percent: return "percent";
    case type_t::period: return "period";
    case type_t::greater_than_equals: return "greater_than_equals";
    case type_t::less_than_equals: return "less_than_equals";
    case type_t::double_equals: return "double_equals";
    case type_t::percent_equals: return "percent_equals";
    case type_t::double_pipe: return "double_pipe";
    case type_t::double_ampersand: return "double_ampersand";
    default: assert(0 && "unknown token");
    }
}
