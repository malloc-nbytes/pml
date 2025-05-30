#include <assert.h>
#include <stdio.h>

#include "parser.h"
#include "lexer.h"
#include "token.h"
#include "gl.h"
#include "err.h"
#include "utils.h"

static Expr *parse_expr(Lexer *l);

static Token *expect(Lexer *l, Token_Type exp) {
        assert(0);
}

static Token *expectkw(Lexer *l, const char *kw) {
        assert(0);
}

static Expr_Let *parse_expr_let(Lexer *l) {
        lexer_discard(l); // let
        Token *id = expect(l, TOKEN_TYPE_IDENTIFIER);
        (void)expect(l, TOKEN_TYPE_EQUALS);
        Expr *e = parse_expr(l);
        (void)expectkw(l, GL_KW_IN);
        Expr *in = parse_expr(l);
        return expr_let_alloc(id->lx.s, id->lx.l, e, in);
}

static Expr_Array parse_comma_sep_exprs(Lexer *l) {
        assert(0);
}

static Expr *parse_primary_expr(Lexer *l) {
        Token *tok = NULL;
        Expr *left = NULL;
        Expr *right = NULL;

        // Unary expressions
        while (lexer_peek(l, 0) && (lexer_peek(l, 0)->ty == TOKEN_TYPE_MINUS
                                 || lexer_peek(l, 0)->ty == TOKEN_TYPE_BANG)) {
                Token *op = lexer_next(l);
                Expr *operand = parse_primary_expr(l);
                left = (Expr *)expr_unary_alloc(operand, op->lx.s, op->lx.l);
        }

        while (1) {
                Token *hd = lexer_peek(l, 0);
                if (!hd) { return left; }
                switch (hd->ty) {
                case TOKEN_TYPE_IDENTIFIER: {
                        left = (Expr *)expr_identifier(hd->lx.s, hd->lx.l);
                        lexer_discard(l); // identifier
                } break;
                case TOKEN_TYPE_LPAREN: {
                        (void)lexer_next(l); // (
                        Expr_Array tuple = parse_comma_sep_exprs(l);
                        (void)expect(l, TOKEN_TYPE_RPAREN);
                        if (tuple.len > 1) {
                                // Tuple
                                //left = expr_tuple_alloc(tuple, tok);
                                assert(0);
                        }
                        else {
                                // Math
                                left = tuple.data[0];
                                dyn_array_free(tuple);
                        }
                } break;
                case TOKEN_TYPE_INTLIT: {
                        Token *intlit = lexer_next(l);
                        assert(intlit->lx.l < 32);
                        char buf[32] = {0};
                        (void)strncpy(buf, intlit->lx.s, intlit->lx.l);
                        left = (Expr *)expr_intlit_alloc(atoi(buf));
                } break;
                case TOKEN_TYPE_STRLIT: {
                        Token *strlit = lexer_next(l);
                        left = (Expr *)expr_strlit_alloc(strlit->lx.s, strlit->lx.l);
                } break;
                case TOKEN_TYPE_KEYWORD: {
                        // TODO: booleans
                        // TODO: None type
                        if (!strcmp(hd->lx.s, GL_KW_IN)) {
                                return left;
                        } else {
                                err_wargs("invalid keyword `%s` in primary expression", utils_tmp_str_wlen(hd->lx.s, hd->lx.l));
                        }
                } break;
                default: goto done;
                }
        }

 done:
        return left;
}

static Expr *parse_multiplicative_expr(Lexer *l) {
    Expr *lhs = parse_primary_expr(l);
    Token *cur = lexer_peek(l, 0);
    while (cur && (cur->ty == TOKEN_TYPE_ASTERISK
                   || cur->ty == TOKEN_TYPE_FORWARD_SLASH
                   || cur->ty == TOKEN_TYPE_PERCENT)) {
        Token* op = lexer_next(l);
        Expr *rhs = parse_primary_expr(l);
        lhs = (Expr *)expr_binary_alloc(lhs, rhs, op->lx.s, op->lx.l);
        cur = lexer_peek(l, 0);
    }
    return lhs;
}

static Expr *parse_additive_expr(Lexer *l) {
    Expr *lhs = parse_multiplicative_expr(l);
    Token *cur = lexer_peek(l, 0);
    while (cur && (cur->ty == TOKEN_TYPE_PLUS
                   || cur->ty == TOKEN_TYPE_MINUS)) {
        Token *op = lexer_next(l);
        Expr *rhs = parse_multiplicative_expr(l);
        lhs = (Expr *)expr_binary_alloc(lhs, rhs, op->lx.s, op->lx.l);
        cur = lexer_peek(l, 0);
    }
    return lhs;
}

static Expr *parse_equalitive_expr(Lexer *l) {
    Expr *lhs = parse_additive_expr(l);
    Token *cur = lexer_peek(l, 0);
    while (cur && (cur->ty == TOKEN_TYPE_DOUBLE_EQUALS
                   || cur->ty == TOKEN_TYPE_GREATER_THAN_EQUALS
                   || cur->ty == TOKEN_TYPE_GREATER_THAN
                   || cur->ty == TOKEN_TYPE_LESS_THAN_EQUALS
                   || cur->ty == TOKEN_TYPE_LESS_THAN
                   || cur->ty == TOKEN_TYPE_BANG_EQUALS)) {
        Token *op = lexer_next(l);
        Expr *rhs = parse_additive_expr(l);
        lhs = (Expr *)expr_binary_alloc(lhs, rhs, op->lx.s, op->lx.l);
        cur = lexer_peek(l, 0);
    }
    return lhs;
}

static Expr *parse_logical_expr(Lexer *l) {
    Expr *lhs = parse_equalitive_expr(l);
    Token *cur = lexer_peek(l, 0);
    while (cur && (cur->ty == TOKEN_TYPE_DOUBLE_AMPERSAND
                   || cur->ty == TOKEN_TYPE_DOUBLE_PIPE)) {
        Token *op = lexer_next(l);
        Expr *rhs = parse_equalitive_expr(l);
        lhs = (Expr *)expr_binary_alloc(lhs, rhs, op->lx.s, op->lx.l);
        cur = lexer_peek(l, 0);
    }
    return lhs;
}

static Expr *parse_bitwise_expr(Lexer *l) {
        return parse_logical_expr(l);
}

static Expr *parse_expr(Lexer *l) {
        Token *hd = lexer_peek(l, 0);
        if (!strcmp(hd->lx.s, GL_KW_LET)) {
                return (Expr *)parse_expr_let(l);
        }
        return parse_bitwise_expr(l);
}

Program parser_parse_program(Lexer *l) {
        Program p = (Program) {
                .exprs = dyn_array_empty(Expr_Array),
        };
        while (lexer_speek(l, 0)->ty != TOKEN_TYPE_EOF) {
                printf("HERE\n");
                dyn_array_append(p.exprs, parse_expr(l));
        }
        return p;
}
