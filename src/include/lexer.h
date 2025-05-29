#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
        Token *hd;
        Token *tl;
} Lexer;

Lexer lexer_analyze(char *src, const char *fp);
void lexer_append(Lexer *l, Token *t);
void lexer_dump(Lexer *l);

#endif // LEXER_Hx
