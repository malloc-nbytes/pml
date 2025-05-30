#ifndef VISITOR_H
#define VISITOR_H

typedef struct Visitor {
        void (*visit_expr_binary)(struct Visitor *, Expr_Binary *e);
} Visitor;

#endif // VISITOR_H
