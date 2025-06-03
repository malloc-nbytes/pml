#include "assert.h"

#include "inter.h"
#include "value.h"
#include "grammar.h"
#include "sem.h"
#include "log.h"

typedef struct {
        char *id;
        RTT *rtt;
        Expr *e;
} Inter_Sym;

typedef struct {
        SMap_Array scope;
        int after_main;
        Value *value;
} Inter_Context;

static Inter_Sym *inter_sym_alloc(char *id, Expr *e, RTT *rtt) {
        Inter_Sym *sym = (Inter_Sym *)malloc(sizeof(Inter_Sym));
        sym->id = id;
        sym->rtt = rtt;
        sym->e = e;
        return sym;
}

void inter_add_sym_to_scope(Inter_Context *ctx, Inter_Sym *sym) {
        LOG_WARGS(LOG_INFO, stdout, "Adding %s to scope", sym->id);
        smap_insert(&ctx->scope.data[ctx->scope.len-1], sym->id, (void *)sym);
}

void push_scope(Inter_Context *ctx) {
        dyn_array_append(ctx->scope, smap_create(NULL, NULL));
}

void pop_scope(Inter_Context *ctx) {
        dyn_array_pop(ctx->scope);
}

Inter_Sym *inter_scope_lookup(const Inter_Context *ctx, const char *id) {
        for (int i = ctx->scope.len-1; i >= 0; --i) {
                if (smap_contains(&ctx->scope.data[i], id)) {
                        return (Inter_Sym *)smap_get(&ctx->scope.data[i], id);
                }
        }
        return NULL;
}

static void inter_visit_expr_bin(Visitor *v, Expr_Binary *e) {
        assert(0);
}

static void inter_visit_expr_unary(Visitor *v, Expr_Unary *e) {
        assert(0);
}

static void inter_visit_expr_let(Visitor *v, Expr_Let *e) {
        Inter_Context *ctx = (Inter_Context *)v->ctx;
        Inter_Sym *sym = inter_sym_alloc(e->id, e->e, e->base.rtt);
        inter_add_sym_to_scope(ctx, sym);

        if (e->in) {
                e->in->accept(e->in, v);
        }
}

static void inter_visit_expr_letfn(Visitor *v, Expr_Letfn *e) {
        Inter_Context *ctx = (Inter_Context *)v->ctx;

        if (!ctx->after_main) {
                Inter_Sym *fnsym = inter_sym_alloc(e->id, e->e, e->base.rtt);
                inter_add_sym_to_scope(ctx, fnsym);
        }

        if (ctx->after_main) {
                push_scope(ctx);
                e->e->accept(e->e, v);
                pop_scope(ctx);
        }
}

static void inter_visit_expr_intlit(Visitor *v, Expr_Intlit *e) {
        Inter_Context *ctx = (Inter_Context *)v->ctx;
        Value *value = (Value *)value_int_alloc(e->i);
        ctx->value = value;
}

static void inter_visit_expr_strlit(Visitor *v, Expr_Strlit *e) {
        assert(0);
}

static void inter_visit_expr_identifier(Visitor *v, Expr_Identifier *e) {
        Inter_Context *ctx = (Inter_Context *)v->ctx;
        Inter_Sym *sym = inter_scope_lookup(ctx, e->id);
        assert(sym);
        sym->e->accept(sym->e, v);
}

static void inter_visit_expr_funccall(Visitor *v, Expr_Funccall *e) {
        e->callee->accept(e->callee, v);
        for (size_t i = 0; i < e->exprs.len; ++i) {
                e->exprs.data[i]->accept(e->exprs.data[i], v);
        }
}

static Visitor *inter_create_visitor(Inter_Context *s) {
        Visitor *v = (Visitor *)malloc(sizeof(Visitor));
        v->ctx = (void *)s;
        v->visit_expr_binary = inter_visit_expr_bin;
        v->visit_expr_unary = inter_visit_expr_unary;
        v->visit_expr_let = inter_visit_expr_let;
        v->visit_expr_letfn = inter_visit_expr_letfn;
        v->visit_expr_intlit = inter_visit_expr_intlit;
        v->visit_expr_strlit = inter_visit_expr_strlit;
        v->visit_expr_identifier = inter_visit_expr_identifier;
        v->visit_expr_funccall = inter_visit_expr_funccall;
        return v;
}

void inter_interpret(Program *p) {
        LOG(LOG_INFO, stdout, "*** Interpreting");

        Inter_Context ctx = (Inter_Context) {
                .scope = dyn_array_empty(SMap_Array),
                .after_main = 0,
                .value = NULL,
        };
        dyn_array_append(ctx.scope, smap_create(NULL, NULL));
        Visitor *v = inter_create_visitor(&ctx);

        Expr *main = NULL;
        for (size_t i = 0; i < p->exprs.len; ++i) {
                Expr *e = p->exprs.data[i];
                if (e->ty == EXPR_TYPE_FUNCCALL
                           && ((Expr_Funccall*)e)->callee->ty == EXPR_TYPE_IDENTIFIER
                           && !strcmp(((Expr_Identifier *)((Expr_Funccall*)e)->callee)->id, "main")) {
                        main = e;
                } else {
                        e->accept(e, v);
                }
        }

        ctx.after_main = 1;
        if (main) {
                main->accept(main, v);
                printf("value: %d\n", ((Value_Int *)ctx.value)->actual.data);
        }
}
