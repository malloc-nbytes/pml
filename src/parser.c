#include <assert.h>
#include <stdio.h>

#include "parser.h"
#include "lexer.h"
#include "token.h"
#include "gl.h"
#include "err.h"
#include "utils.h"

typedef struct {
        Lexer *l;
        int global_scope;
} Parsing_Context;

static Expr *parse_expr(Parsing_Context *ctx);

static Token *expect(Lexer *l, Token_Type exp) {
        Token *hd = lexer_next(l);
        if (hd->ty != exp) {
                err_wargs("expected token `%s` but got `%s`",
                          token_type_to_cstr(exp), token_type_to_cstr(hd->ty));
        }
        return hd;
}

static Token *expectkw(Parsing_Context *ctx, const char *kw) {
        Token *hd = lexer_next(ctx->l);
        if (hd->ty != TOKEN_TYPE_KEYWORD || strncmp(hd->lx.s, kw, hd->lx.l) != 0) {
                err_wargs("expected keyword `%s` but got `%s`",
                          kw, utils_tmp_str_wlen(hd->lx.s, hd->lx.l));
        }
        return hd;
}

static Expr_Let *parse_expr_let(Parsing_Context *ctx) {
        lexer_discard(ctx->l); // let
        Token *id = expect(ctx->l, TOKEN_TYPE_IDENTIFIER);
        (void)expect(ctx->l, TOKEN_TYPE_EQUALS);
        Expr *e = parse_expr(ctx);
        (void)expectkw(ctx, GL_KW_IN);
        Expr *in = parse_expr(ctx);
        return expr_let_alloc(id->lx.s, id->lx.l, e, in);
}

static Expr_Array parse_comma_sep_exprs(Parsing_Context *ctx) {
        assert(0);
}

static Expr *parse_primary_expr(Parsing_Context *ctx) {
        Token *tok = NULL;
        Expr *left = NULL;
        Expr *right = NULL;

        // Unary expressions
        while (lexer_peek(ctx->l, 0) && (lexer_peek(ctx->l, 0)->ty == TOKEN_TYPE_MINUS
                                 || lexer_peek(ctx->l, 0)->ty == TOKEN_TYPE_BANG)) {
                Token *op = lexer_next(ctx->l);
                Expr *operand = parse_primary_expr(ctx);
                left = (Expr *)expr_unary_alloc(operand, op->lx.s, op->lx.l);
        }

        while (1) {
                Token *hd = lexer_peek(ctx->l, 0);
                if (!hd) { return left; }
                switch (hd->ty) {
                case TOKEN_TYPE_IDENTIFIER: {
                        left = (Expr *)expr_identifier(hd->lx.s, hd->lx.l);
                        lexer_discard(ctx->l); // identifier
                } break;
                case TOKEN_TYPE_LPAREN: {
                        (void)lexer_next(ctx->l); // (
                        Expr_Array tuple = parse_comma_sep_exprs(ctx);
                        (void)expect(ctx->l, TOKEN_TYPE_RPAREN);
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
                        Token *intlit = lexer_next(ctx->l);
                        assert(intlit->lx.l < 32);
                        char buf[32] = {0};
                        (void)strncpy(buf, intlit->lx.s, intlit->lx.l);
                        left = (Expr *)expr_intlit_alloc(atoi(buf));
                } break;
                case TOKEN_TYPE_STRLIT: {
                        Token *strlit = lexer_next(ctx->l);
                        left = (Expr *)expr_strlit_alloc(strlit->lx.s, strlit->lx.l);
                } break;
                case TOKEN_TYPE_KEYWORD: {
                        // TODO: booleans
                        // TODO: None type
                        if (!strncmp(hd->lx.s, GL_KW_IN, hd->lx.l)) {
                                return left;
                        } else {
                                err_wargs("invalid keyword `%s` in primary expression", utils_tmp_str_wlen(hd->lx.s, hd->lx.l));
                        }
                } break;
                /* case TOKEN_TYPE_NEWLINE: { */
                /*         lexer_discard(ctx->l); // \n */
                /*         if (!left) { */
                /*                 err("could not parse primary expression, hit newline"); */
                /*         } */
                /*         goto done; */
                /* } break; */
                default: goto done;
                }
        }

 done:
        return left;
}

static Expr *parse_multiplicative_expr(Parsing_Context *ctx) {
    Expr *lhs = parse_primary_expr(ctx);
    Token *cur = lexer_peek(ctx->l, 0);
    while (cur && (cur->ty == TOKEN_TYPE_ASTERISK
                   || cur->ty == TOKEN_TYPE_FORWARD_SLASH
                   || cur->ty == TOKEN_TYPE_PERCENT)) {
        Token* op = lexer_next(ctx->l);
        Expr *rhs = parse_primary_expr(ctx);
        lhs = (Expr *)expr_binary_alloc(lhs, rhs, op->lx.s, op->lx.l);
        cur = lexer_peek(ctx->l, 0);
    }
    return lhs;
}

static Expr *parse_additive_expr(Parsing_Context *ctx) {
    Expr *lhs = parse_multiplicative_expr(ctx);
    Token *cur = lexer_peek(ctx->l, 0);
    while (cur && (cur->ty == TOKEN_TYPE_PLUS
                   || cur->ty == TOKEN_TYPE_MINUS)) {
        Token *op = lexer_next(ctx->l);
        Expr *rhs = parse_multiplicative_expr(ctx);
        lhs = (Expr *)expr_binary_alloc(lhs, rhs, op->lx.s, op->lx.l);
        cur = lexer_peek(ctx->l, 0);
    }
    return lhs;
}

static Expr *parse_equalitive_expr(Parsing_Context *ctx) {
    Expr *lhs = parse_additive_expr(ctx);
    Token *cur = lexer_peek(ctx->l, 0);
    while (cur && (cur->ty == TOKEN_TYPE_DOUBLE_EQUALS
                   || cur->ty == TOKEN_TYPE_GREATER_THAN_EQUALS
                   || cur->ty == TOKEN_TYPE_GREATER_THAN
                   || cur->ty == TOKEN_TYPE_LESS_THAN_EQUALS
                   || cur->ty == TOKEN_TYPE_LESS_THAN
                   || cur->ty == TOKEN_TYPE_BANG_EQUALS)) {
        Token *op = lexer_next(ctx->l);
        Expr *rhs = parse_additive_expr(ctx);
        lhs = (Expr *)expr_binary_alloc(lhs, rhs, op->lx.s, op->lx.l);
        cur = lexer_peek(ctx->l, 0);
    }
    return lhs;
}

static Expr *parse_logical_expr(Parsing_Context *ctx) {
    Expr *lhs = parse_equalitive_expr(ctx);
    Token *cur = lexer_peek(ctx->l, 0);
    while (cur && (cur->ty == TOKEN_TYPE_DOUBLE_AMPERSAND
                   || cur->ty == TOKEN_TYPE_DOUBLE_PIPE)) {
        Token *op = lexer_next(ctx->l);
        Expr *rhs = parse_equalitive_expr(ctx);
        lhs = (Expr *)expr_binary_alloc(lhs, rhs, op->lx.s, op->lx.l);
        cur = lexer_peek(ctx->l, 0);
    }
    return lhs;
}

static Expr *parse_bitwise_expr(Parsing_Context *ctx) {
        return parse_logical_expr(ctx);
}

static Expr *parse_expr(Parsing_Context *ctx) {
        Token *hd = lexer_peek(ctx->l, 0);
        if (!strncmp(hd->lx.s, GL_KW_LET, hd->lx.l)) {
                return (Expr *)parse_expr_let(ctx);
        }
        Expr *e = parse_bitwise_expr(ctx);
        assert(e);
        return e;
}

Program parser_parse_program(Lexer *l) {
        Program p = (Program) { .exprs = dyn_array_empty(Expr_Array) };

        Parsing_Context ctx = (Parsing_Context) {
                .l = l,
                .global_scope = 0,
        };

        while (lexer_speek(ctx.l, 0)->ty != TOKEN_TYPE_EOF) {
                dyn_array_append(p.exprs, parse_expr(&ctx));
        }
        return p;
}
