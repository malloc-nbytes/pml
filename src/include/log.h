#ifndef LOG_H
#define LOG_H

#define LOG_ERR  "ERR"
#define LOG_WARN "WARN"
#define LOG_INFO "INFO"
#define LOG_TODO "TODO"

#ifdef DEBUG
#include <stdio.h>
#include <string.h>
#define LOG(lvl, stream, msg)                                   \
        do {                                                    \
                fprintf(stream, "[" lvl "]: " msg "\n");        \
                if (!strcmp(lvl, LOG_ERR)) { exit(1); }         \
        } while (0)
#define LOG_WARGS(lvl, stream, msg, ...)                                \
        do {                                                            \
                fprintf(stream, "[" lvl "]: " msg "\n", __VA_ARGS__);   \
                if (!strcmp(lvl, LOG_ERR)) { exit(1); }                 \
        } while (0)
#else
#define LOG(_0, _1, _2) (void)0
#define LOG_WARGS(_0, _1, _2, ...) (void)0
#endif

#endif // LOG_H
