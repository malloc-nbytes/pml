#include <cassert>

#include "parser.hxx"
#include "grammar.hxx"

static expr_t *parse_assignment_expr(lexer_t *l) {
    expr_t *lhs = parse_logical_expr(l);
    token_t *cur = lexer_peek(l);
    if (!cur) return lhs;

    switch (cur->ty) {
    case TT_EQUALS: {
    }
    default: return lhs;
    }
    assert(0 && "unreachable");
}

static expr_t *parse_expr(lexer_t *l) {
    return parse_assignment_expr(l);
}

program_t parser_parse(lexer_t *l) {
    std::vector<expr_t *> exprs;

    while (lexer_peek(l, 0)->ty != TT_EOF) {
        exprs.push_back(parse_expr(l));
    }

    return (program_t) {
        .exprs = exprs,
    };
}
