#include "tokens.hh"
#include "lexer.hh"
#include "debug.hh"
#include "asm.hh"
#include "parser.hh"
#include <iostream>

namespace s = assembly;

std::list<std::string> binary_rax_rbx(std::string does) {
    std::list<std::string> inst;
    inst.push_back(s::pop(s::rbx));
    inst.push_back(s::pop(s::rax));
    inst.push_back(does);
    inst.push_back(s::push(s::rax));

    return inst;
}

int main() {
    std::map<std::string, std::list<std::string>> macros;

    macros["+"] = binary_rax_rbx(s::add(s::rax, s::rbx));
    macros["-"] = binary_rax_rbx(s::sub(s::rax, s::rbx));
    macros["^"] = binary_rax_rbx(s::xor_(s::rax, s::rbx));
    macros["*"] = binary_rax_rbx(s::mul(s::rbx));
    macros["/"] = binary_rax_rbx(s::div(s::rbx));

    std::cerr << "Parsing test.forth" << std::endl;
    lexer l("../test.forth");
    std::list<token::token> tokens = l.lex();

    /*for (const auto& t : tokens) {
        debug(t);
    }*/

    parser p(tokens, macros);
    auto f = p.parse();

    for (auto fn : f) {
        std::cout << fn.assemble();
    }
}
