#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "sem.h"
#include "grammar.h"
#include "ds/smap.h"

static void sem_visit_expr_identifier(Visitor *v, Expr_Identifier *e) {
        Sem_Scope *s = (Sem_Scope *)v->ctx;
        assert(0);
}

static void sem_visit_expr_strlit(Visitor *v, Expr_Strlit *e) {
        Sem_Scope *s = (Sem_Scope *)v->ctx;
        assert(0);
}

static void sem_visit_expr_intlit(Visitor *v, Expr_Intlit *e) {
        Sem_Scope *s = (Sem_Scope *)v->ctx;
        assert(0);
}

static void sem_visit_expr_let(Visitor *v, Expr_Let *e) {
        Sem_Scope *s = (Sem_Scope *)v->ctx;
        assert(0);
}

static void sem_visit_expr_unary(Visitor *v, Expr_Unary *e) {
        Sem_Scope *s = (Sem_Scope *)v->ctx;
        assert(0);
}

static void sem_visit_expr_bin(Visitor *v, Expr_Binary *e) {
        Sem_Scope *s = (Sem_Scope *)v->ctx;
        assert(0);
}

static Visitor *sem_create_visitor(Sem_Scope *s) {
        Visitor *v = (Visitor *)malloc(sizeof(Visitor));
        v->ctx = (void *)s;
        v->visit_expr_binary = sem_visit_expr_bin;
        v->visit_expr_unary = sem_visit_expr_unary;
        v->visit_expr_let = sem_visit_expr_let;
        v->visit_expr_intlit = sem_visit_expr_intlit;
        v->visit_expr_strlit = sem_visit_expr_strlit;
        v->visit_expr_identifier = sem_visit_expr_identifier;
        return v;
}

Sem_Scope semantic_analyze(Program *p) {
        Sem_Scope s = (Sem_Scope) {
                .scope = dyn_array_empty(SMap_Array),
                .errs = dyn_array_empty(Str_Array),
        };
        dyn_array_append(s.scope, smap_create(NULL, NULL));

        Visitor *v = sem_create_visitor(&s);

        for (size_t i = 0; i < p->exprs.len; ++i) {
                p->exprs.data[i]->accept(p->exprs.data[i], v);
        }

        return s;
}
