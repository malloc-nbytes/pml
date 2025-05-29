#include <assert.h>
#include <stdio.h>

#include "parser.h"
#include "lexer.h"
#include "token.h"
#include "gl.h"

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
                if (!lexer_peek(l, 0)) { return left; }
                switch (lexer_peek(l, 0)->ty) {
                case TOKEN_TYPE_IDENTIFIER: {
                        ;
                } break;
                case TOKEN_TYPE_LPAREN: {
                        ;
                } break;
                case TOKEN_TYPE_INTLIT: {
                        ;
                } break;
                case TOKEN_TYPE_STRLIT: {
                        ;
                } break;
                case TOKEN_TYPE_KEYWORD: {
                        ;
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
                dyn_array_append(p.exprs, parse_expr(l));
        }
        return p;
}
