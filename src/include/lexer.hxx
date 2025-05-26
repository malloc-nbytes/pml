#ifndef LEXER_HXX
#define LEXER_HXX

#include "token.hxx"

typedef struct {
    token_t *hd;
    token_t *tl;
} lexer_t;

lexer_t lexer_analyze(const char *fp, char *src);
void lexer_dump(lexer_t *l);
token_t *lexer_peek(const lexer_t *l, int p);
token_t *lexer_next(lexer_t *l);
void lexer_discard(lexer_t *l);

#endif // LEXER_HXX
