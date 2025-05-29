#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "ds/arrays.h"

typedef enum {
        EXPR_TYPE_BINARY,
        EXPR_TYPE_UNARY,
        EXPR_TYPE_LET,
        EXPR_TYPE_INTLIT,
        EXPR_TYPE_STRLIT,
        EXPR_TYPE_IDENTIFIER,
} Expr_Type;

typedef struct {
        Expr_Type ty;
} Expr;

typedef struct {
        Expr base;
        Expr *l;
        Expr *r;
        Char_Array op;
} Expr_Binary;

typedef struct {
        Expr base;
        Char_Array op;
        Expr *e;
} Expr_Unary;

typedef struct {
        Expr base;
        Char_Array id;
        Expr *e;
        Expr *in;
} Expr_Let;

typedef struct {
        Expr base;
        int i;
} Expr_Intlit;

typedef struct {
        Expr base;
        Char_Array s;
} Expr_Strlit;

typedef struct {
        Expr base;
        Char_Array id;
} Expr_Identifier;

#endif // GRAMMAR_H
