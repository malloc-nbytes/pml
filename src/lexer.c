#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "lexer.h"
#include "err.h"
#include "gl.h"
#include "ds/smap.h"

static SMap g_ops;
static SMap g_kws;

static void init_ops(void) {
        static Token_Type lparen = {TOKEN_TYPE_LPAREN};
        static Token_Type rparen = {TOKEN_TYPE_RPAREN};
        static Token_Type lcurly = {TOKEN_TYPE_LCURLY_BRACKET};
        static Token_Type rcurly = {TOKEN_TYPE_RCURLY_BRACKET};
        static Token_Type lsqr = {TOKEN_TYPE_LSQR_BRACKET};
        static Token_Type rsqr = {TOKEN_TYPE_RSQR_BRACKET};
        static Token_Type plus = {TOKEN_TYPE_PLUS};
        static Token_Type minus = {TOKEN_TYPE_MINUS};
        static Token_Type asterisk = {TOKEN_TYPE_ASTERISK};
        static Token_Type forward_slash = {TOKEN_TYPE_FORWARD_SLASH};
        static Token_Type percent = {TOKEN_TYPE_PERCENT};
        static Token_Type equals = {TOKEN_TYPE_EQUALS};
        static Token_Type comma = {TOKEN_TYPE_COMMA};
        static Token_Type semicolon = {TOKEN_TYPE_SEMICOLON};
        static Token_Type colon = {TOKEN_TYPE_COLON};
        static Token_Type dollar = {TOKEN_TYPE_DOLLAR};
        static Token_Type greater_than = {TOKEN_TYPE_GREATER_THAN};
        static Token_Type less_than = {TOKEN_TYPE_LESS_THAN};
        static Token_Type greater_than_equals = {TOKEN_TYPE_GREATER_THAN_EQUALS};
        static Token_Type less_than_equals = {TOKEN_TYPE_LESS_THAN_EQUALS};
        static Token_Type double_equals = {TOKEN_TYPE_DOUBLE_EQUALS};
        static Token_Type double_pipe = {TOKEN_TYPE_DOUBLE_PIPE};
        static Token_Type double_ampersand = {TOKEN_TYPE_DOUBLE_AMPERSAND};

        g_ops = smap_create(NULL, NULL);
        smap_insert(&g_ops, "(", &lparen);
        smap_insert(&g_ops, ")", &rparen);
        smap_insert(&g_ops, "{", &lcurly);
        smap_insert(&g_ops, "}", &rcurly);
        smap_insert(&g_ops, "[", &lsqr);
        smap_insert(&g_ops, "]", &rsqr);
        smap_insert(&g_ops, "+", &plus);
        smap_insert(&g_ops, "-", &minus);
        smap_insert(&g_ops, "*", &asterisk);
        smap_insert(&g_ops, "/", &forward_slash);
        smap_insert(&g_ops, "%", &percent);
        smap_insert(&g_ops, "=", &equals);
        smap_insert(&g_ops, ",", &comma);
        smap_insert(&g_ops, ";", &semicolon);
        smap_insert(&g_ops, ":", &colon);
        smap_insert(&g_ops, "$", &dollar);
        smap_insert(&g_ops, ">", &greater_than);
        smap_insert(&g_ops, "<", &less_than);
        smap_insert(&g_ops, ">=", &greater_than_equals);
        smap_insert(&g_ops, "<=", &less_than_equals);
        smap_insert(&g_ops, "==", &double_equals);
        smap_insert(&g_ops, "||", &double_pipe);
        smap_insert(&g_ops, "&&", &double_ampersand);
}

static Token_Type detop(const char *s, size_t *len) {
#define MAX 32
        assert(*len < MAX);
        for (int i = ((int)(*len)); i >= 0; --i) {
                char buf[MAX] = {0};
                (void)strncpy(buf, s, i);
                if (smap_contains(&g_ops, buf)) {
                        return *(Token_Type *)smap_get(&g_ops, buf);
                }
        }
        return TOKEN_TYPE_UNKNOWN;
#undef MAX
}

Token *lexer_peek(const Lexer *l, int p) {
        Token *it = l->hd;
        while (it && p-- >= 0) { it = it->n; }
        return it;
}

void lexer_append(Lexer *l, Token *t) {
        if (!l->hd) {
                l->hd = t;
                l->tl = l->hd;
        } else {
                l->tl->n = t;
                l->tl = l->tl->n;
        }
}

void lexer_dump(Lexer *l) {
        Token *t = l->hd;
        while (t) {
                printf("Token: {\n");
                if (t->ty == TOKEN_TYPE_NEWLINE) {
                        printf("  lx: \\n\n");
                } else {
                        printf("  lx: %.*s\n", (int)t->lx.l, t->lx.s);
                }
                printf("  ty: %s\n", token_type_to_cstr(t->ty));
                printf("  r: %d\n", (int)t->r);
                printf("  c: %d\n", (int)t->c);
                printf("}\n");
                t = t->n;
        }
}

static int iskw(const char *s, size_t len) {
        const char *kws[] = GL_KWS;
        for (size_t i = 0; i < sizeof(kws)/sizeof(*kws); ++i) {
                size_t j;
                for (j = 0; j < len; ++j) {
                        if (kws[i][j] != s[j]) { break; }
                }
                if (j == len && kws[i][j] == '\0') {
                        return 1;
                }
        }
        return 0;
}

static size_t consume_until(
    const char *s,
    const int (*pred)(int))
{
    size_t i;
    int skip = 0;
    for (i = 0; s[i]; ++i) {
        if (!skip && pred(s[i])) { return i; }
        if (skip && s[i] == '\\') { skip = 0; }
        else if (s[i] == '\\') { skip = 1; }
        else { skip = 0; }
    }
    return i;
}

static int not_op(int c) {
        return isalnum(c)
                || (char)c == '_'
                || (char)c == '\n'
                || (char)c == '\t'
                || (char)c == '\r'
                || (char)c == ' ';
}
static int not_digit(int c) { return !isdigit(c); }
static int not_ident(int c) { return !isalnum(c) && (char)c != '_'; }
static int is_double_quote(int c) { return (char)c == '"'; }
static int is_single_quote(int c) { return (char)c == '\''; }

Lexer lexer_analyze(char *src, const char *fp) {
        Lexer lexer = (Lexer) {
                .hd = NULL,
                .tl = NULL,
        };

        init_ops();

        size_t i = 0, r = 1, c = 1;
        while (src[i]) {
                char ch = src[i];
                Token *t = NULL;
                if (ch == ' ' || ch == '\t') {
                        ++i, ++c;
                } else if (ch == '\n') {
                        lexer_append(&lexer, token_alloc(src+i, 1, TOKEN_TYPE_NEWLINE, r, c, fp));
                        ++i, ++r, c = 1;
                } else if (ch == '_' || isalpha(ch)) {
                        size_t len = consume_until(src+i, not_ident);
                        if (iskw(src+i, len)) {
                                lexer_append(&lexer, token_alloc(src+i, len, TOKEN_TYPE_KEYWORD, r, c, fp));
                        } else {
                                lexer_append(&lexer, token_alloc(src+i, len, TOKEN_TYPE_IDENTIFIER, r, c, fp));
                        }
                        i += len, c += len;
                } else if (ch == '"' || ch == '\'') {
                        size_t len;
                        if (ch == '"') {
                                len = consume_until(src+i+1, is_double_quote);
                        } else {
                                len = consume_until(src+i+1, is_single_quote);
                        }
                        lexer_append(&lexer, token_alloc(src+i+1, len, TOKEN_TYPE_STRLIT, r, c, fp));
                        i += len+2, c += len+2;
                } else if (isdigit(ch)) {
                        size_t len = consume_until(src+i, not_digit);
                        lexer_append(&lexer, token_alloc(src+i, len, TOKEN_TYPE_INTLIT, r, c, fp));
                        i += len, c += len;
                } else {
                        size_t len = consume_until(src+i, not_op);
                        Token_Type ty = detop(src+i, &len);
                        if (ty == TOKEN_TYPE_UNKNOWN) {
                                err_wargs("invalid token at %s", src+i);
                        }
                        lexer_append(&lexer, token_alloc(src+i, len, ty, r, c, fp));
                        i += len, c += len;
                }
        }

        lexer_append(&lexer, token_alloc("EOF", 3, TOKEN_TYPE_EOF, r, c, fp));

        return lexer;
}
