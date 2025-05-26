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

using namespace lexer;

static std::unordered_map<std::string_view, token::type_t> g_ops = {
    {"+", token::type_t::plus},
    {"-", token::type_t::minus},
    {"*", token::type_t::asterisk},
    {"/", token::type_t::forward_slash},
    {">", token::type_t::greater_than},
    {"<", token::type_t::less_than},
    {"=", token::type_t::equals},
    {"%", token::type_t::percent},
    {".", token::type_t::period},
    {"(", token::type_t::lparen},
    {")", token::type_t::rparen},
    {"[", token::type_t::lsqr_bracket},
    {"]", token::type_t::rsqr_bracket},
    {"{", token::type_t::lcurly_bracket},
    {"}", token::type_t::rcurly_bracket},
    {"$", token::type_t::dollar},
    {";", token::type_t::semicolon},
    {",", token::type_t::comma},
    {">=", token::type_t::greater_than_equals},
    {"<=", token::type_t::less_than_equals},
    {"==", token::type_t::double_equals},
    {"%=", token::type_t::percent_equals},
    {"||", token::type_t::double_pipe},
    {"&&", token::type_t::double_ampersand},
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

static void append(lexer_t *l, token::token_t *t) {
    if (!l->hd) {
        l->hd = t;
        l->tl = l->hd;
    } else {
        l->tl->n = t;
        l->tl = l->tl->n;
    }
}

void lexer::dump(lexer_t *l) {
    auto t = l->hd;
    while (t) {
        printf("Token: {\n");
        if (t->ty == token::type_t::newline) {
            printf("  lx: \\n\n");
        } else {
            printf("  lx: %.*s\n", (int)t->lx.l, t->lx.st);
        }
        printf("  ty: %d\n", (int)t->ty);
        printf("  r: %d\n", (int)t->r);
        printf("  c: %d\n", (int)t->c);
        printf("}\n");
        t = t->n;
    }
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

static std::pair<token::type_t, size_t> determine_type_from_op(
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
    return {token::type_t::unknown, 0};
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

lexer_t lexer::lex(const char *fp, char *src) {
    lexer_t lx = (lexer_t) {
        .hd = nullptr,
        .tl = nullptr,
    };

    int i = 0, r = 1, c = 1;
    while (src[i]) {
        token::token_t *t = nullptr;
        char ch = src[i];

        if (ch == ' ' || ch == '\t') {
            ++i, ++c;
        } else if (ch == '\n') {
            append(&lx, token::create(src+i, 1, token::type_t::newline, r, c));
            ++i, ++r, c = 1;
        } else if (isalpha(ch) || ch == '_') {
            size_t l = consume_until(src+i, [](char c) { return !isalnum(c) && c != '_'; });
            append(&lx, token::create(src+i, l, iskw(src+i, l)
                                      ? token::type_t::keyword
                                      : token::type_t::identifier,
                                      r, c));
            i += l, c += l;
        } else if (isdigit(ch)) {
            size_t l = consume_until(src+i, [](char c) { return !isdigit(c); });
            append(&lx, token::create(src+i, l, token::type_t::intlit, r, c));
            i += l, c += l;
        } else if (ch == '"' || ch == '\'') {
            size_t l = consume_until(src+i+1, [=](char c) {
                return ch == '"'
                    ? c == '"'
                    : c == '\'';
            });
            append(&lx, token::create(src+i+1, l, token::type_t::strlit, r, c));
            i += l+2, c += l+2;
        } else {
            size_t len = consume_until(src+i, [](char c) { return !isop(c); });
            auto [ty, sz] = determine_type_from_op(src+i, len);
            if (sz == 0) {
                fprintf(stderr, "Invalid operator at row %d, col %d: %.*s\n", r, c, (int)len, src+i);
                exit(1);
            }
            append(&lx, token::create(src+i, sz, ty, r, c));
            i += sz, c += sz;
        }
    }

    return lx;
}

