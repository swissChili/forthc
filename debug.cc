#include <iostream>
#include "debug.hh"

void debug(token::token tok) {
    auto t = tok.node;
    if (auto v = std::get_if<token::whole>(&t))
        std::cout << "whole\t\t" << v->val;
    else if (auto v = std::get_if<token::word>(&t))
        std::cout << "word\t\t" << v->val;
    else if (auto v = std::get_if<token::start_fn>(&t))
        std::cout << "start_fn\t" << ":";
    else if (auto v = std::get_if<token::end_fn>(&t))
        std::cout << "end_fn\t\t" << ";";
    else if (auto v = std::get_if<token::string>(&t))
        std::cout << "string\t\t" << v->val;
    else if (auto v = std::get_if<token::eof>(&t))
        std::cout << "<EOF>";
    std::cout << std::endl;
}

unsigned get_line(token::token t) {
    return t.line;
}
