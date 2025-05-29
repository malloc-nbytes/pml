#ifndef ERR_H
#define ERR_H

#include <stdio.h>
#include <stdlib.h>

#define err_wargs(msg, ...)                     \
    do {                                        \
        fprintf(stderr, "error: " msg, __VA_ARGS__);    \
        fprintf(stderr, "\n");                  \
        exit(1);                                \
    } while (0)

#define err(msg)                                \
    do {                                        \
        fprintf(stderr, "error: ");             \
        fprintf(stderr, msg);                   \
        fprintf(stderr, "\n");                  \
        exit(1);                                \
    } while (0)


#endif // ERR_H
