#include "tokens.hh"
#include "lexer.hh"
#include "debug.hh"
#include "asm.hh"
#include "parser.hh"
#include <iostream>

namespace s = assembly;

int main() {
    std::cout << "Parsing test.forth" << std::endl;
    lexer l("../test.forth");
    std::list<token::token> tokens = l.lex();

    for (auto t : tokens) {
        debug(t);
    }

    parser p{tokens};
    auto f = p.parse();

    for (auto fn : f) {
        std::cout << fn.assemble();
    }
}
