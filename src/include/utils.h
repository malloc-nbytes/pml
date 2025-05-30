#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

unsigned long djb2(const char *str);
char *utils_tmp_str_wlen(const char *s, size_t n);

#endif // UTILS_H
