#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "ds/arrays.h"
#include "rtt.h"

typedef struct Visitor Visitor;

typedef enum {
        EXPR_TYPE_BINARY,
        EXPR_TYPE_UNARY,
        EXPR_TYPE_LET,
        EXPR_TYPE_INTLIT,
        EXPR_TYPE_STRLIT,
        EXPR_TYPE_IDENTIFIER,
} Expr_Type;

typedef struct Expr {
        Expr_Type ty;
        void (*accept)(struct Expr *, Visitor *v);
        RTT *rtt;
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

typedef struct Visitor {
        void *ctx;

        void (*visit_expr_binary)(Visitor *, Expr_Binary *e);
        void (*visit_expr_unary)(Visitor *, Expr_Unary *e);
        void (*visit_expr_let)(Visitor *, Expr_Let *e);
        void (*visit_expr_intlit)(Visitor *, Expr_Intlit *e);
        void (*visit_expr_strlit)(Visitor *, Expr_Strlit *e);
        void (*visit_expr_identifier)(Visitor *, Expr_Identifier *e);
} Visitor;

// void visit_expr_binary(Visitor *v, Expr_Binary *e);
// void visit_expr_unary(Visitor *v, Expr_Unary *e);
// void visit_expr_let(Visitor *v, Expr_Let *e);
// void visit_expr_intlit(Visitor *v, Expr_Intlit *e);
// void visit_expr_strlit(Visitor *v, Expr_Strlit *e);
// void visit_expr_identifier(Visitor *v, Expr_Identifier *e);

#endif // GRAMMAR_H
