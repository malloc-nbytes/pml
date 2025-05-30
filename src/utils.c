#include <string.h>
#include <assert.h>

#include "utils.h"

unsigned long djb2(const char *str) {
        unsigned long hash = 5381;
        int c;

        while ((c = *str++))
                hash = ((hash << 5) + hash) + c;

        return hash;
}

char *utils_tmp_str_wlen(const char *s, size_t n) {
        static char buf[256] = {0};
        (void)strncpy(buf, s, n);
        return buf;
}
