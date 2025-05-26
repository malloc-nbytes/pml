#include <cassert>
#include <iostream>

#include "lexer.hxx"
#include "gl.hxx"

#define CIO_IMPL
#include "cio.h"

int main(void) {
    const char *input_fp = "input.txt";
    assert(cio_file_exists(input_fp));

    size_t src_len = 0;
    char *src = cio_file_to_cstr_wnewlines(input_fp, &src_len);

    auto l = lexer::lex(input_fp, src);
    lexer::dump(&l);


    return 0;
}
