#include <stdlib.h>

#include "grammar.h"
#include "dyn_array.h"
#include "err.h"
#include "ds/arrays.h"

Expr *expr_alloc(Expr_Type ty) {
        Expr *e;
        switch (ty) {
        case EXPR_TYPE_BINARY: {
                e = malloc(sizeof(Expr_Binary));
                e->ty = EXPR_TYPE_BINARY;
        } break;
        case EXPR_TYPE_UNARY: {
                e = malloc(sizeof(Expr_Unary));
                e->ty = EXPR_TYPE_UNARY;
        } break;
        case EXPR_TYPE_LET: {
                e = malloc(sizeof(Expr_Let));
                e->ty = EXPR_TYPE_LET;
        } break;
        case EXPR_TYPE_INTLIT: {
                e = malloc(sizeof(Expr_Intlit));
                e->ty = EXPR_TYPE_INTLIT;
        } break;
        case EXPR_TYPE_STRLIT: {
                e = malloc(sizeof(Expr_Strlit));
                e->ty = EXPR_TYPE_STRLIT;
        } break;
        case EXPR_TYPE_IDENTIFIER: {
                e = malloc(sizeof(Expr_Identifier));
                e->ty = EXPR_TYPE_IDENTIFIER;
        } break;
        default: {
                err_wargs("unknown expr type: %d", (int)ty);
        } break;
        }
        return e;
}

#define array_copy(id, cond, next, da, src)     \
        for (size_t (id) = 0; (cond); next) {   \
                dyn_array_append(da, (src)[i]); \
        }

Expr_Binary *expr_binary_alloc(Expr *l, Expr *r, const char *op, size_t len) {
        Expr_Binary *e = (Expr_Binary *)expr_alloc(EXPR_TYPE_BINARY);
        e->l = l;
        e->r = r;
        e->op = dyn_array_empty(Char_Array);
        array_copy(i, i < len, ++i, e->op, op);
        return e;
}

Expr_Unary *expr_unary_alloc(Expr *e, const char *op, size_t len) {
        Expr_Unary *u = (Expr_Unary *)expr_alloc(EXPR_TYPE_UNARY);
        u->e = e;
        u->op = dyn_array_empty(Char_Array);
        array_copy(i, i < len, ++i, u->op, op);
        return u;
}

Expr_Let *expr_let_alloc(const char *id, size_t len, Expr *e, Expr *in) {
        Expr_Let *l = (Expr_Let *)expr_alloc(EXPR_TYPE_LET);
        l->id = dyn_array_empty(Char_Array);
        array_copy(i, i < len, ++i, l->id, id);
        l->e = e;
        l->in = in;
        return l;
}

Expr_Intlit *expr_intlit_alloc(int i) {
        Expr_Intlit *e = (Expr_Intlit *)expr_alloc(EXPR_TYPE_INTLIT);
        e->i = i;
        return e;
}

Expr_Strlit *expr_strlit_alloc(const char *s, size_t len) {
        Expr_Strlit *e = (Expr_Strlit *)expr_alloc(EXPR_TYPE_STRLIT);
        e->s = dyn_array_empty(Char_Array);
        array_copy(i, i < len, ++i, e->s, s);
        return e;
}

Expr_Identifier *expr_identifier(const char *id, size_t len) {
        Expr_Identifier *e = (Expr_Identifier *)expr_alloc(EXPR_TYPE_IDENTIFIER);
        e->id = dyn_array_empty(Char_Array);
        array_copy(i, i < len, ++i, e->id, id);
        return e;
}
