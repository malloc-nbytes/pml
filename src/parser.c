#include <assert.h>

#include "parser.h"
#include "lexer.h"

Expr *parse_expr(Lexer *l) {
        assert(0);
}

Program parser_parse_program(Lexer *l) {
        Program p = (Program) {
                .exprs = dyn_array_empty(Expr_Array),
        };
        while (lexer_speek(l, 0)->ty != TOKEN_TYPE_EOF) {
                dyn_array_append(p.exprs, parse_expr(l));
        }
        return p;
}
