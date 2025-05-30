#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "ds/smap.h"
#include "sem.h"
#define CIO_IMPL
#include "cio.h"

int main(void) {
        const char *fp = "input.txt";

        size_t src_len;
        char *src = cio_file_to_cstr_wnewlines(fp, &src_len);

        Lexer l = lexer_analyze(src, fp);
        lexer_dump(&l);

        Program p = parser_parse_program(&l);

        return 0;
}
