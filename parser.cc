#include <iostream>
#include <list>
#include <variant>
#include "parser.hh"
#include "asm.hh"
#include "debug.hh"

void parser::error(std::string message, token::token &t) {
    std::cerr
        << "Error parsing line " << get_line(t) << ": " << message << std::endl;
    throw message;
}

bool parser::parse_instruction(s::function &fn) {
    if (auto word = std::get_if<token::word>(&tokens.front())) {
        if (macro_words.find(word->val) != macro_words.end()) {
            fn << macro_words[word->val];
        } else
            fn << s::call(word->val);
        return true;
    } else if (auto whole = std::get_if<token::whole>(&tokens.front())) {
        fn << s::push(whole->val);
        return true;
    } else return false;
}

s::function parser::parse_function() {
    if (std::get_if<token::start_fn>(&tokens.front()) == nullptr) {
        error("Expected a function", tokens.front());
    }
    tokens.pop_front();

    auto name_word = std::get_if<token::word>(&tokens.front());
    if (!name_word)
        error("Expected a word as the function name", tokens.front());

    s::function fn{name_word->val};

    do tokens.pop_front();
    while (parse_instruction(fn));

    if (!std::get_if<token::end_fn>(&tokens.front()))
        error("Expected to end function", tokens.front());

    tokens.pop_front();

    return fn;
}

std::list<s::function> parser::parse() {
    std::list<s::function> fns;

    // While the file isn't over
    while (std::get_if<token::eof>(&tokens.front()) == nullptr) {
        fns.push_back(parse_function());
    }
    return fns;
}
