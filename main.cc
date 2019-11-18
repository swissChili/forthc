#include "tokens.hh"
#include "lexer.hh"
#include "debug.hh"
#include "asm.hh"
#include "parser.hh"
#include <iostream>

namespace s = assembly;

int main() {
    std::map<std::string, std::list<std::string>> macros;

    std::list<std::string> add_inst;
    add_inst.push_back(s::pop(s::rax));
    add_inst.push_back(s::pop(s::rbx));
    add_inst.push_back(s::add(s::rax, s::rbx));
    add_inst.push_back(s::push(s::rax));

    macros["+"] = add_inst;

    std::list<std::string> dup_inst;
    dup_inst.push_back(s::pop(s::rax));
    dup_inst.push_back(s::push(s::rax));
    dup_inst.push_back(s::push(s::rax));

    macros["dup"] = dup_inst;

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
