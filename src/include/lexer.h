#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#define lexer_speek(l, p) lexer_peek(l, p) && lexer_peek(l, p)

typedef struct {
        Token *hd;
        Token *tl;
} Lexer;

Lexer lexer_analyze(char *src, const char *fp);
void lexer_append(Lexer *l, Token *t);
void lexer_dump(Lexer *l);
Token *lexer_peek(const Lexer *l, int p);

#endif // LEXER_Hx
