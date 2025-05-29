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
Expr *expr_alloc(Expr_Type ty);

typedef struct {
        Expr base;
        Expr *l;
        Expr *r;
        Char_Array op;
} Expr_Binary;
Expr_Binary *expr_binary_alloc(Expr *l, Expr *r, const char *op, size_t len);

typedef struct {
        Expr base;
        Char_Array op;
        Expr *e;
} Expr_Unary;
Expr_Unary *expr_unary_alloc(Expr *e, const char *op, size_t len);

typedef struct {
        Expr base;
        Char_Array id;
        Expr *e;
        Expr *in;
} Expr_Let;
Expr_Let *expr_let_alloc(const char *id, size_t len, Expr *e, Expr *in);

typedef struct {
        Expr base;
        int i;
} Expr_Intlit;
Expr_Intlit *expr_intlit_alloc(int i);

typedef struct {
        Expr base;
        Char_Array s;
} Expr_Strlit;
Expr_Strlit *expr_strlit_alloc(const char *s, size_t len);

typedef struct {
        Expr base;
        Char_Array id;
} Expr_Identifier;
Expr_Identifier *expr_identifier(const char *id, size_t len);

DYN_ARRAY_TYPE(Expr *, Expr_Array);

typedef struct {
        Expr_Array exprs;
} Program;

#endif // GRAMMAR_H
