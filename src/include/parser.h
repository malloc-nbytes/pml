#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "grammar.h"

Program parser_parse_program(Lexer *l);

#endif // PARSER_H
