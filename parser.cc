#include <iostream>
#include <list>
#include <variant>
#include "parser.hh"
#include "asm.hh"

void parser::error(std::string message) {
    std::cerr
        << "Error parsing line " << line << ": " << message << std::endl;
    throw message;
}

s::function parser::parse_function() {

}

void parser::parse() {
    std::list<s::function> fns;

    while (std::get_if<token::eol>(&tokens.front()) != nullptr) {
        fns.push_back(parse_function());
    }
}
