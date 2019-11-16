#include "tokens.hh"
#include "lexer.hh"
#include "debug.hh"
#include <iostream>

int main() {
    std::cout << "Parsing test.forth" << std::endl;
    lexer l("../test.forth");
    std::list<token::token> tokens = l.lex();

    for (auto t : tokens) {
        debug(t);
    }
}
