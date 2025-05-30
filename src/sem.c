#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "sem.h"
#include "grammar.h"
#include "ds/smap.h"

void sem_add_sym_to_scope(Sem_Scope *ss, Sem_Sym *sym) {
        smap_insert(&ss->scope.data[ss->scope.len-1], sym->id, (void *)sym);
}

// NOTE: makes a copy of `msg`.
static void pusherr(Sem_Scope *scope, char *msg) {
        dyn_array_append(scope->errs, strdup(msg));
}

Sem_Sym *sem_scope_lookup(const Sem_Scope *ss, const char *id) {
        for (int i = ss->scope.len-1; i >= 0; --i) {
                if (smap_contains(&ss->scope.data[i], id)) {
                        return (Sem_Sym *)smap_get(&ss->scope.data[i], id);
                }
        }

        return NULL;
}

static void sem_visit_expr_identifier(Visitor *v, Expr_Identifier *e) {
        Sem_Scope *s = (Sem_Scope *)v->ctx;
        Sem_Sym *sym = sem_scope_lookup(s, e->id);
        if (!sym) {
                char buf[256] = {0};
                sprintf(buf, "undefined identifier: %s", e->id);
                pusherr(s, buf);
        } else {
                e->base.rtt = sym->rtt;
        }
}

static void sem_visit_expr_strlit(Visitor *v, Expr_Strlit *e) {
        Sem_Scope *s = (Sem_Scope *)v->ctx;
        assert(0);
}

static void sem_visit_expr_intlit(Visitor *v, Expr_Intlit *e) {
        Sem_Scope *s = (Sem_Scope *)v->ctx;
        e->base.rtt = rtt_alloc(RTT_INT);
}

static void sem_visit_expr_let(Visitor *v, Expr_Let *e) {
        Sem_Scope *s = (Sem_Scope *)v->ctx;

        e->e->accept(e->e, v);

        Sem_Sym *sym = (Sem_Sym *)malloc(sizeof(Sem_Sym));
        sym->id = e->id;
        sym->rtt = e->base.rtt;
        sem_add_sym_to_scope(s, sym);

        e->in->accept(e->in, v);
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
