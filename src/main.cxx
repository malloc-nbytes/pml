#include <cassert>
#include <iostream>

#include "lexer.hxx"
#include "parser.hxx"
#include "gl.hxx"

#define CIO_IMPL
#include "cio.h"

int main(void) {
    const char *input_fp = "input.txt";
    assert(cio_file_exists(input_fp));

    size_t src_len = 0;
    char *src = cio_file_to_cstr_wnewlines(input_fp, &src_len);

    auto l = lexer_analyze(input_fp, src);
    //lexer_dump(&l);
    program_t p = parser_parse(&l);

    return 0;
}
