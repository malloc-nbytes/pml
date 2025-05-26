#ifndef GRAMMAR_HXX
#define GRAMMAR_HXX

#include <vector>

typedef enum {
    ET_BINARY,
    ET_UNARY,
    ET_STRLIT,
    ET_INTLIT,
    ET_IDENTIFIER,
} expr_type_t;

typedef struct {
    expr_type_t ty;
} expr_t;

typedef struct {
    expr_t b;
    expr_t *l;
    expr_t *r;
} expr_binary_t;

typedef struct {
    expr_t b;
    char op;
    expr_t e;
} expr_unary_t;

typedef struct {
    expr_t b;
    char *s;
    size_t len;
} expr_strlit_t;

typedef struct {
    expr_t b;
    int i;
} expr_intlit_t;

typedef struct {
    expr_t b;
    char *id;
    size_t len;
} expr_identifier_t;

typedef struct {
    std::vector<expr_t *> exprs;
} program_t;

#endif // GRAMMAR_HXX
