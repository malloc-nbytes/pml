#ifndef GRAMMAR_HXX
#define GRAMMAR_HXX

#include "ds/array.hxx"

namespace grammar {
    namespace expr {
        typedef enum {
            binary,
            unary,
            strlit,
            intlit,
            identifier,
        } type_t;

        typedef struct {
            type_t ty;
        } expr_t;

        typedef struct {
            expr_t b;
            expr_t *l;
            expr_t *r;
        } binary_t;

        typedef struct {
            expr_t b;
            char op;
            expr_t e;
        } unary_t;

        typedef struct {
            expr_t b;
            char *s;
            size_t len;
        } strlit_t;

        typedef struct {
            expr_t b;
            int i;
        } intlit_t;

        typedef struct {
            expr_t b;
            char *id;
            size_t len;
        } identifier_t;
    };

    typedef struct {
        array<expr::expr_t *> exprs;
    } program_t;
};

#endif // GRAMMAR_HXX
