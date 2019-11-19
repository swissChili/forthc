#include <iostream>
#include "debug.hh"

void debug(token::token t) {
    if (auto v = std::get_if<token::whole>(&t))
        std::cout << "whole\t\t" << v->val;
    else if (auto v = std::get_if<token::word>(&t))
        std::cout << "word\t\t" << v->val;
    else if (auto v = std::get_if<token::start_fn>(&t))
        std::cout << "start_fn\t" << ":";
    else if (auto v = std::get_if<token::end_fn>(&t))
        std::cout << "end_fn\t\t" << ";";
    else if (auto v = std::get_if<token::eof>(&t))
        std::cout << "<EOF>";
    std::cout << std::endl;
}

unsigned get_line(token::token t) {
    if (auto v = std::get_if<token::whole>(&t))
        return v->line;
    else if (auto v = std::get_if<token::word>(&t))
        return v->line;
    else if (auto v = std::get_if<token::start_fn>(&t))
        return v->line;
    else if (auto v = std::get_if<token::end_fn>(&t))
        return v->line;
    else if (auto v = std::get_if<token::eof>(&t))
        return v->line;

    // Execution should never get here, but im doing this to fix the warning
    return 0;
}
