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

