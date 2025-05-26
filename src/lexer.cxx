#include <iostream>
#include <cassert>
#include <functional>
#include <unordered_set>
#include <string_view>

#include <string.h>
#include <ctype.h>

#include "lexer.hxx"
#include "token.hxx"
#include "gl.hxx"

static std::unordered_map<std::string_view, token_type_t> g_ops = {
    {"+", TT_PLUS},
    {"-", TT_MINUS},
    {"*", TT_ASTERISK},
    {"/", TT_FORWARD_SLASH},
    {">", TT_GREATER_THAN},
    {"<", TT_LESS_THAN},
    {"=", TT_EQUALS},
    {"%", TT_PERCENT},
    {".", TT_PERIOD},
    {"(", TT_LPAREN},
    {")", TT_RPAREN},
    {"[", TT_LSQR_BRACKET},
    {"]", TT_RSQR_BRACKET},
    {"{", TT_LCURLY_BRACKET},
    {"}", TT_RCURLY_BRACKET},
    {"$", TT_DOLLAR},
    {";", TT_SEMICOLON},
    {",", TT_COMMA},
    {">=", TT_GREATER_THAN_EQUALS},
    {"<=", TT_LESS_THAN_EQUALS},
    {"==", TT_DOUBLE_EQUALS},
    {"%=", TT_PERCENT_EQUALS},
    {"||", TT_DOUBLE_PIPE},
    {"&&", TT_DOUBLE_AMPERSAND},
};

constexpr size_t get_max_operator_length() {
    constexpr std::string_view ops[] = {
        "+", "-", "*", "/", ">", "<", "=", "%", ".",
        ">=", "<=", "==", "%=", "||", "&&", "(", ")",
        "[", "]", "{", "}", "$", ";",
    };
    size_t max_len = 0;
    for (const auto& op : ops) {
        max_len = std::max(max_len, op.length());
    }
    return max_len;
}

static void append(lexer_t *l, token_t *t) {
    if (!l->hd) {
        l->hd = t;
        l->tl = l->hd;
    } else {
        l->tl->n = t;
        l->tl = l->tl->n;
    }
}

void lexer_dump(lexer_t *l) {
    auto t = l->hd;
    while (t) {
        printf("Token: {\n");
        if (t->ty == token_type_t::TT_NEWLINE) {
            printf("  lx: \\n\n");
        } else {
            printf("  lx: %.*s\n", (int)t->lx.l, t->lx.st);
        }
        printf("  ty: %s\n", token_type_to_cstr(t->ty));
        printf("  r: %d\n", (int)t->r);
        printf("  c: %d\n", (int)t->c);
        printf("}\n");
        t = t->n;
    }
}

token_t *lexer_peek(const lexer_t *l, int p) {
    int i = 0;
    token_t *it = l->hd;
    while (it && i < p) {
        ++i;
        it = it->n;
    }
    return it;
}

token_t *lexer_next(lexer_t *l) {
    token_t *it = l->hd;
    l->hd = l->hd->n;
    return it;
}

void lexer_discard(lexer_t *l) {
    l->hd = l->hd->n;
}

static bool iskw(const char *s, size_t len) {
    const char *kws[] = GL_KWDS;
    for (size_t i = 0; i < sizeof(kws)/sizeof(*kws); ++i) {
        size_t j;
        for (j = 0; j < len; ++j) {
            if (kws[i][j] != s[j]) { break; }
        }
        if (j == len && kws[i][j] == '\0') {
            return true;
        }
    }
    return false;
}

static bool isop(char c) {
        return !isdigit(c)
                && !isalpha(c)
                && c != '_'
                && c != ' '
                && c != '\n'
                && c != '\t'
                && c != '\r';
}

static std::pair<token_type_t, size_t> determine_type_from_op(
    const char *src,
    size_t max_len)
{
    const size_t N = 2;
    static_assert(get_max_operator_length() <= N, "Longest operator length exceeds 2");
    for (size_t len = std::min(max_len, size_t(N)); len > 0; --len) {
        std::string_view op(src, len);
        auto it = g_ops.find(op);
        if (it != g_ops.end()) {
            return {it->second, len};
        }
    }
    return {TT_UNKNOWN, 0};
}

static size_t consume_until(
    const char *s,
    const std::function<bool(char)> &predicate)
{
    size_t i;
    bool skip = false;
    for (i = 0; s[i]; ++i) {
        if (!skip && predicate(s[i])) { return i; }
        if (skip && s[i] == '\\') { skip = false; }
        else if (s[i] == '\\') { skip = true; }
        else { skip = false; }
    }
    return i;
}

lexer_t lexer_analyze(const char *fp, char *src) {
    lexer_t lx = (lexer_t) {
        .hd = nullptr,
        .tl = nullptr,
    };

    int i = 0, r = 1, c = 1;
    while (src[i]) {
        token_t *t = nullptr;
        char ch = src[i];

        if (ch == ' ' || ch == '\t') {
            ++i, ++c;
        } else if (ch == '\n') {
            append(&lx, token_create(src+i, 1, TT_NEWLINE, r, c));
            ++i, ++r, c = 1;
        } else if (isalpha(ch) || ch == '_') {
            size_t l = consume_until(src+i, [](char c) { return !isalnum(c) && c != '_'; });
            append(&lx, token_create(src+i, l, iskw(src+i, l)
                                      ? TT_KEYWORD
                                      : TT_IDENTIFIER,
                                      r, c));
            i += l, c += l;
        } else if (isdigit(ch)) {
            size_t l = consume_until(src+i, [](char c) { return !isdigit(c); });
            append(&lx, token_create(src+i, l, TT_INTLIT, r, c));
            i += l, c += l;
        } else if (ch == '"' || ch == '\'') {
            size_t l = consume_until(src+i+1, [=](char c) {
                return ch == '"'
                    ? c == '"'
                    : c == '\'';
            });
            append(&lx, token_create(src+i+1, l, TT_STRLIT, r, c));
            i += l+2, c += l+2;
        } else {
            size_t len = consume_until(src+i, [](char c) { return !isop(c); });
            auto [ty, sz] = determine_type_from_op(src+i, len);
            if (sz == 0) {
                fprintf(stderr, "Invalid operator at row %d, col %d: %.*s\n", r, c, (int)len, src+i);
                exit(1);
            }
            append(&lx, token_create(src+i, sz, ty, r, c));
            i += sz, c += sz;
        }
    }

    append(&lx, token_create(src+i, 0, TT_EOF, r, c));

    return lx;
}

