#include <stdio.h>

#include "lexer.h"
#include "ds/smap.h"
#define CIO_IMPL
#include "cio.h"

int main(void) {
        const char *fp = "input.txt";

        size_t src_len;
        char *src = cio_file_to_cstr_wnewlines(fp, &src_len);

        Lexer l = lexer_analyze(src, fp);
        lexer_dump(&l);

        return 0;
}
