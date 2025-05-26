#ifndef LEXER_HXX
#define LEXER_HXX

#include "token.hxx"

namespace lexer {
    typedef struct {
        token::token_t *hd;
        token::token_t *tl;
    } lexer_t;

    lexer_t lex(const char *fp, char *src);
    void dump(lexer_t *l);
    token::token_t *peek(const lexer_t *l, int p);
    token::token_t *next(lexer_t *l);
    void discard(lexer_t *l);
};

#endif // LEXER_HXX
