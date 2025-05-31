#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "ds/arrays.h"
#include "rtt.h"

typedef struct Visitor Visitor;

typedef enum {
        EXPR_TYPE_BINARY,
        EXPR_TYPE_UNARY,
        EXPR_TYPE_LET,
        EXPR_TYPE_LETFN,
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

DYN_ARRAY_TYPE(Expr *, Expr_Array);

typedef struct {
        Expr base;
        Expr *l;
        Expr *r;
        char *op;
} Expr_Binary;
Expr_Binary *expr_binary_alloc(Expr *l, Expr *r, char *op, size_t len);

typedef struct {
        Expr base;
        char *op;
        Expr *e;
} Expr_Unary;
Expr_Unary *expr_unary_alloc(Expr *e, char *op, size_t len);

typedef struct {
        Expr base;
        char *id;
        Expr *e;
        Expr *in; // optional, if NULL, it is global
} Expr_Let;
Expr_Let *expr_let_alloc(char *id, size_t len, Expr *e, Expr *in);

typedef struct {
        Expr base;
        char *id;
        Expr_Array params;
        Expr *e;
        Expr *in; // optional, if NULL, it is global
} Expr_Letfn;
Expr_Letfn *expr_letfn_alloc(
        char *id,
        size_t len,
        Expr **params,
        size_t params_len,
        Expr *e,
        Expr *in
);

typedef struct {
        Expr base;
        int i;
} Expr_Intlit;
Expr_Intlit *expr_intlit_alloc(int i);

typedef struct {
        Expr base;
        char *s;
} Expr_Strlit;
Expr_Strlit *expr_strlit_alloc(char *s, size_t len);

typedef struct {
        Expr base;
        char *id;
} Expr_Identifier;
Expr_Identifier *expr_identifier(char *id, size_t len);

typedef struct {
        Expr_Array exprs;
} Program;

typedef struct Visitor {
        void *ctx;

        void (*visit_expr_binary)(Visitor *, Expr_Binary *e);
        void (*visit_expr_unary)(Visitor *, Expr_Unary *e);
        void (*visit_expr_let)(Visitor *, Expr_Let *e);
        void (*visit_expr_letfn)(Visitor *, Expr_Letfn *e);
        void (*visit_expr_intlit)(Visitor *, Expr_Intlit *e);
        void (*visit_expr_strlit)(Visitor *, Expr_Strlit *e);
        void (*visit_expr_identifier)(Visitor *, Expr_Identifier *e);
} Visitor;

#endif // GRAMMAR_H
