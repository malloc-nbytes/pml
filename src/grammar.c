#include <stdlib.h>

#include "grammar.h"
#include "dyn_array.h"
#include "err.h"
#include "utils.h"
#include "ds/arrays.h"

/////////////////////////
// Accept Functions
/////////////////////////

void accept_expr_binary(Expr *e, Visitor *v) {
        if (v->visit_expr_binary) {
                v->visit_expr_binary(v, (Expr_Binary *)e);
        }
}

void accept_expr_unary(Expr *e, Visitor *v) {
        if (v->visit_expr_unary) {
                v->visit_expr_unary(v, (Expr_Unary *)e);
        }
}

void accept_expr_let(Expr *e, Visitor *v) {
        if (v->visit_expr_let) {
                v->visit_expr_let(v, (Expr_Let *)e);
        }
}

void accept_expr_letfn(Expr *e, Visitor *v) {
        if (v->visit_expr_letfn) {
                v->visit_expr_letfn(v, (Expr_Letfn *)e);
        }
}

void accept_expr_intlit(Expr *e, Visitor *v) {
        if (v->visit_expr_intlit) {
                v->visit_expr_intlit(v, (Expr_Intlit *)e);
        }
}

void accept_expr_strlit(Expr *e, Visitor *v) {
        if (v->visit_expr_strlit) {
                v->visit_expr_strlit(v, (Expr_Strlit *)e);
        }
}

void accept_expr_identifier(Expr *e, Visitor *v) {
        if (v->visit_expr_identifier) {
                v->visit_expr_identifier(v, (Expr_Identifier *)e);
        }
}

/////////////////////////
// End Accept Functions
/////////////////////////

/////////////////////////
// Visitor Functions
/////////////////////////

void visit_expr_binary(Visitor *v, Expr_Binary *e) {
        e->l->accept(e->l, v);
        e->r->accept(e->r, v);
}

void visit_expr_unary(Visitor *v, Expr_Unary *e) {
        e->e->accept(e->e, v);
}

void visit_expr_let(Visitor *v, Expr_Let *e) {
        e->e->accept(e->e, v);
        if (e->in) {
                e->in->accept(e->in, v);
        }
}

void visit_expr_letfn(Visitor *v, Expr_Letfn *e) {
        e->e->accept(e->e, v);
        if (e->in) {
                e->in->accept(e->in, v);
        }
}

void visit_expr_intlit(Visitor *v, Expr_Intlit *e) {}

void visit_expr_strlit(Visitor *v, Expr_Strlit *e) {}

void visit_expr_identifier(Visitor *v, Expr_Identifier *e) {}

/////////////////////////
// End Visitor Functions
/////////////////////////

Expr *expr_alloc(Expr_Type ty) {
        Expr *e;
        switch (ty) {
        case EXPR_TYPE_BINARY: {
                e = (Expr *)malloc(sizeof(Expr_Binary));
                e->ty = EXPR_TYPE_BINARY;
                e->accept = accept_expr_binary;
        } break;
        case EXPR_TYPE_UNARY: {
                e = (Expr *)malloc(sizeof(Expr_Unary));
                e->ty = EXPR_TYPE_UNARY;
                e->accept = accept_expr_unary;
        } break;
        case EXPR_TYPE_LET: {
                e = (Expr *)malloc(sizeof(Expr_Let));
                e->ty = EXPR_TYPE_LET;
                e->accept = accept_expr_let;
        } break;
        case EXPR_TYPE_LETFN: {
                e = (Expr *)malloc(sizeof(Expr_Letfn));
                e->ty = EXPR_TYPE_LETFN;
                e->accept = accept_expr_letfn;
        } break;
        case EXPR_TYPE_INTLIT: {
                e = (Expr *)malloc(sizeof(Expr_Intlit));
                e->ty = EXPR_TYPE_INTLIT;
                e->accept = accept_expr_intlit;
        } break;
        case EXPR_TYPE_STRLIT: {
                e = (Expr *)malloc(sizeof(Expr_Strlit));
                e->ty = EXPR_TYPE_STRLIT;
                e->accept = accept_expr_strlit;
        } break;
        case EXPR_TYPE_IDENTIFIER: {
                e = (Expr *)malloc(sizeof(Expr_Identifier));
                e->ty = EXPR_TYPE_IDENTIFIER;
                e->accept = accept_expr_identifier;
        } break;
        default: {
                err_wargs("unknown expr type: %d", (int)ty);
        } break;
        }
        e->rtt = NULL;
        return e;
}

Expr_Binary *expr_binary_alloc(Expr *l, Expr *r, char *op, size_t len) {
        Expr_Binary *e = (Expr_Binary *)expr_alloc(EXPR_TYPE_BINARY);
        e->l = l;
        e->r = r;
        e->op = strndup(op, len);
        return e;
}

Expr_Unary *expr_unary_alloc(Expr *e, char *op, size_t len) {
        Expr_Unary *u = (Expr_Unary *)expr_alloc(EXPR_TYPE_UNARY);
        u->e = e;
        u->op = strndup(op, len);
        return u;
}

Expr_Let *expr_let_alloc(char *id, size_t len, Expr *e, Expr *in) {
        Expr_Let *l = (Expr_Let *)expr_alloc(EXPR_TYPE_LET);
        l->id = strndup(id, len);
        l->e = e;
        l->in = in;
        return l;
}

Expr_Letfn *expr_letfn_alloc(
        char *id,
        size_t len,
        char **params,
        size_t *param_names_len,
        size_t params_len,
        Expr *e,
        Expr *in)
{
        Expr_Letfn *l = (Expr_Letfn *)expr_alloc(EXPR_TYPE_LETFN);
        l->id = strndup(id, len);
        l->e = e;
        l->in = in;
        l->params = dyn_array_empty(Str_Array);
        for (size_t i = 0; i < params_len; ++i) {
                dyn_array_append(l->params,
                                 strdup(utils_tmp_str_wlen(params[i],
                                                           param_names_len[i])));
        }
        return l;
}


Expr_Intlit *expr_intlit_alloc(int i) {
        Expr_Intlit *e = (Expr_Intlit *)expr_alloc(EXPR_TYPE_INTLIT);
        e->i = i;
        return e;
}

Expr_Strlit *expr_strlit_alloc(char *s, size_t len) {
        Expr_Strlit *e = (Expr_Strlit *)expr_alloc(EXPR_TYPE_STRLIT);
        e->s = strndup(s, len);
        return e;
}

Expr_Identifier *expr_identifier(char *id, size_t len) {
        Expr_Identifier *e = (Expr_Identifier *)expr_alloc(EXPR_TYPE_IDENTIFIER);
        e->id = strndup(id, len);
        return e;
}
