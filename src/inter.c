#include "assert.h"

#include "inter.h"
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
} Inter_Context;

static void inter_visit_expr_bin(Visitor *v, Expr_Binary *e) {
        assert(0);
}

static void inter_visit_expr_unary(Visitor *v, Expr_Unary *e) {
        assert(0);
}

static void inter_visit_expr_let(Visitor *v, Expr_Let *e) {
        assert(0);
}

static void inter_visit_expr_letfn(Visitor *v, Expr_Letfn *e) {
        assert(0);
}

static void inter_visit_expr_intlit(Visitor *v, Expr_Intlit *e) {
        assert(0);
}

static void inter_visit_expr_strlit(Visitor *v, Expr_Strlit *e) {
        assert(0);
}

static void inter_visit_expr_identifier(Visitor *v, Expr_Identifier *e) {
        assert(0);
}

static void inter_visit_expr_funccall(Visitor *v, Expr_Funccall *e) {
        assert(0);
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
        };
        dyn_array_append(ctx.scope, smap_create(NULL, NULL));
        Visitor *v = inter_create_visitor(&ctx);

        Expr *main = NULL;
        for (size_t i = 0; i < p->exprs.len; ++i) {
                if (p->exprs.data[i]->ty == EXPR_TYPE_LETFN
                    && !strcmp(((Expr_Letfn *)p->exprs.data[i])->id, "main")) {
                        main = p->exprs.data[i];
                }
        }

        if (main) {
                main->accept(main, v);
        }
}
