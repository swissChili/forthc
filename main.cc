#include "tokens.hh"
#include "lexer.hh"
#include "debug.hh"
#include "asm.hh"
#include <iostream>

namespace s = assembly;

int main() {
    std::cout << "Parsing test.forth" << std::endl;
    lexer l("../test.forth");
    std::list<token::token> tokens = l.lex();

    for (auto t : tokens) {
        debug(t);
    }

    s::function fn{"main"};
    fn << s::push(240)
       << s::mov(s::rax, 230);

    fn.strings["a"] = "Hello, World";

    std::cout << fn.assemble();
}
