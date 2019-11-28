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

std::list<std::string> call_alias(std::string named) {
    std::list<std::string> inst;
    inst.push_back(s::call(named));
    return inst;
}

int main(int argc, char **argv) {
    std::map<std::string, std::list<std::string>> macros;

    macros["+"] = binary_rax_rbx(s::add(s::rax, s::rbx));
    macros["-"] = binary_rax_rbx(s::sub(s::rax, s::rbx));
    macros["^"] = binary_rax_rbx(s::xor_(s::rax, s::rbx));
    macros["*"] = binary_rax_rbx(s::mul(s::rbx));
    macros["/"] = binary_rax_rbx(s::div(s::rbx));
    macros["="] = call_alias("equal");
    macros["<=>"] = call_alias("spaceship");
    macros["<"] = call_alias("lessthan");
    macros[">"] = call_alias("greaterthan");
    macros["."] = call_alias("puti");
    macros[".-string"] = call_alias("puts");
    macros["!"] = call_alias("setptr");
    macros["@"] = call_alias("getptr");
    macros["c@"] = call_alias("getbyteptr");

    std::string file = "-";
    if (argc > 1) {
        file = argv[1];
    }

    lexer l(file);
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
