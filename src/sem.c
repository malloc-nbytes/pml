#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "sem.h"
#include "grammar.h"
#include "log.h"
#include "ds/smap.h"

static Sem_Sym *sem_sym_alloc(char *id, RTT *rtt) {
        Sem_Sym *sym = (Sem_Sym *)malloc(sizeof(Sem_Sym));
        sym->id = id;
        sym->rtt = rtt;
        return sym;
}

void sem_add_sym_to_scope(Sem_Scope *ss, Sem_Sym *sym) {
        LOG_WARGS(LOG_INFO, stdout, "adding %s to scope", sym->id);
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

        LOG_WARGS(LOG_INFO, stdout, "Identifier: %s, found: %d", e->id, sym != NULL);

        if (!sym) {
                char buf[256] = {0};
                sprintf(buf, "undefined identifier: %s", e->id);
                pusherr(s, buf);
                e->base.rtt = rtt_alloc(RTT_UNIT);
        } else {
                e->base.rtt = sym->rtt;
        }
}

static void sem_visit_expr_strlit(Visitor *v, Expr_Strlit *e) {
        Sem_Scope *s = (Sem_Scope *)v->ctx;
        assert(0);
}

static void sem_visit_expr_intlit(Visitor *v, Expr_Intlit *e) {
        e->base.rtt = rtt_alloc(RTT_INT);
}

static void sem_visit_expr_let(Visitor *v, Expr_Let *e) {
        Sem_Scope *s = (Sem_Scope *)v->ctx;

        e->e->accept(e->e, v);
        e->base.rtt = e->e->rtt;

        Sem_Sym *sym = sem_sym_alloc(e->id, e->base.rtt);
        sem_add_sym_to_scope(s, sym);

        LOG_WARGS(LOG_INFO, stdout, "let: %s, type: %d", e->id, (int)e->e->rtt->kind);

        if (e->in) {
                e->in->accept(e->in, v);
        }
}

static void sem_visit_expr_letfn(Visitor *v, Expr_Letfn *e) {
        Sem_Scope *s = (Sem_Scope *)v->ctx;

        dyn_array_append(s->scope, smap_create(NULL, NULL));

        RTT_Function *fn_type = (RTT_Function *)rtt_alloc(RTT_FUNCTION);
        fn_type->orig_id = e->id;
        fn_type->ptys = dyn_array_empty(RTT_Ptr_Array);

        for (size_t i = 0; i < e->params.len; i++) {
                //RTT *param_type = rtt_alloc(RTT_UNIT); // Placeholder type
                RTT *param_type = NULL;
                dyn_array_append(fn_type->ptys, param_type);
                Sem_Sym *param_sym = sem_sym_alloc(e->params.data[i], param_type);
                sem_add_sym_to_scope(s, param_sym);
        }

        // Visit the function body to determine its return type
        e->e->accept(e->e, v);
        fn_type->rty = e->e->rtt ? e->base.rtt : rtt_alloc(RTT_UNIT); // Default to RTT_UNIT if body has no type

        // Set the RTT of the letfn expression itself
        e->base.rtt = (RTT *)fn_type;

        // Register the function in the outer scope
        Sem_Sym *fn_sym = sem_sym_alloc(e->id, (RTT *)fn_type);
        // Remove the inner scope temporarily to access the outer scope
        SMap temp = dyn_array_pop(s->scope);
        sem_add_sym_to_scope(s, fn_sym);
        // Restore the inner scope
        dyn_array_append(s->scope, temp);

        // Pop the function's parameter scope
        dyn_array_pop(s->scope);

        // Visit the 'in' expression if it exists
        if (e->in) {
                e->in->accept(e->in, v);
        }
}

static void sem_visit_expr_unary(Visitor *v, Expr_Unary *e) {
        Sem_Scope *s = (Sem_Scope *)v->ctx;
        assert(0);
}

static void sem_visit_expr_bin(Visitor *v, Expr_Binary *e) {
        LOG(LOG_INFO, stdout, "Binary expression");

        Sem_Scope *s = (Sem_Scope *)v->ctx;
        e->l->accept(e->l, v);
        e->r->accept(e->r, v);

        if (!e->l->rtt || !e->r->rtt) {
                char buf[256] = {0};
                sprintf(buf, "Binary expression has undefined type for %s operand",
                        !e->l->rtt ? "left" : "right");
                pusherr(s, buf);
                e->base.rtt = rtt_alloc(RTT_UNIT); // Default type
                return;
        }

        if (!rttcompat(e->l->rtt, e->r->rtt)) {
                char buf[256] = {0};
                sprintf(buf, "Type %d is not compatible with type %d for operator %s",
                        (int)e->l->rtt->kind, (int)e->r->rtt->kind, e->op);
                pusherr(s, buf);
                e->base.rtt = rtt_alloc(RTT_UNIT); // Default type
                return;
        }

        // Set the result type based on the operator
        if (strcmp(e->op, "+") == 0 && e->l->rtt->kind == RTT_INT && e->r->rtt->kind == RTT_INT) {
                e->base.rtt = rtt_alloc(RTT_INT);
        } else {
                // Handle other operators or type combinations
                char buf[256] = {0};
                sprintf(buf, "Unsupported operator %s for types %d and %d",
                        e->op, (int)e->l->rtt->kind, (int)e->r->rtt->kind);
                pusherr(s, buf);
                e->base.rtt = rtt_alloc(RTT_UNIT); // Default type
        }
}

static void sem_visit_expr_funccall(Visitor *v, Expr_Funccall *e) {
        Sem_Scope *s = (Sem_Scope *)v->ctx;
        e->callee->accept(e->callee, v);

        if (!e->callee->rtt) {
                pusherr(s, "Function call to undefined identifier");
                return;
        }

        if (e->callee->rtt->kind != RTT_FUNCTION) {
                char buf[256] = {0};
                sprintf(buf, "Callee is not a function, found type: %d", (int)e->callee->rtt->kind);
                pusherr(s, buf);
                return;
        }

        RTT_Function *fn_type = (RTT_Function *)e->callee->rtt;

        if (e->exprs.len != fn_type->ptys.len) {
                char buf[256] = {0};
                sprintf(buf, "Number of function parameters (%zu) does not match (%zu)",
                        e->exprs.len, fn_type->ptys.len);
                pusherr(s, buf);
                e->base.rtt = fn_type->rty;
                return;
        }

        // Process arguments
        for (size_t i = 0; i < e->exprs.len; i++) {
                e->exprs.data[i]->accept(e->exprs.data[i], v);
                fn_type->ptys.data[i] = e->exprs.data[i]->rtt;
        }

        // Set the return type of the function call
        e->base.rtt = fn_type->rty;
}

static Visitor *sem_create_visitor(Sem_Scope *s) {
        Visitor *v = (Visitor *)malloc(sizeof(Visitor));
        v->ctx = (void *)s;
        v->visit_expr_binary = sem_visit_expr_bin;
        v->visit_expr_unary = sem_visit_expr_unary;
        v->visit_expr_let = sem_visit_expr_let;
        v->visit_expr_letfn = sem_visit_expr_letfn;
        v->visit_expr_intlit = sem_visit_expr_intlit;
        v->visit_expr_strlit = sem_visit_expr_strlit;
        v->visit_expr_identifier = sem_visit_expr_identifier;
        v->visit_expr_funccall = sem_visit_expr_funccall;
        return v;
}

Sem_Scope semantic_analyze(Program *p) {
        LOG(LOG_INFO, stdout, "*** Semantic Analysis");

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
