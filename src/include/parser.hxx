#ifndef PARSER_HXX
#define PARSER_HXX

#include "lexer.hxx"
#include "grammar.hxx"

namespace parser { grammar::program_t *parse(lexer::lexer_t *l); };

#endif // PARSER_HXX
