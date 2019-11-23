#include <iostream>
#include <list>
#include <variant>
#include "parser.hh"
#include "asm.hh"
#include "debug.hh"
#include "random.hh"

void parser::error(std::string message, token::token &t) {
    std::cerr
        << "Error parsing line " << get_line(t) << ": " << message << std::endl;
    throw message;
}

bool parser::parse_instruction(s::function &fn) {
    if (auto word = std::get_if<token::word>(&tokens.front())) {
        if (macro_words.find(word->val) != macro_words.end()) {
            fn << macro_words[word->val];
        } else if (word->val == "if") {
            std::string label = random_string();

            std::string else_label = fn.name + "_else_" + label;
            std::string then_label = fn.name + "_then_" + label;

            fn << s::pop(s::rax)
               << "\tcmpq $0, %rax\n"
               << "\tje " + else_label
               << fn.name + "_if_" + label + ":";

            do tokens.pop_front();
            while (parse_instruction(fn));

            fn << "\tjmp " + then_label;

            // TODO: Remove repeated code
            if (auto condition = std::get_if<token::word>(&tokens.front())) {
                if (condition->val == "then") {
                    fn << else_label + ":" << then_label + ":";
                    return true;
                } else if (condition->val == "else") {
                    fn << else_label + ":";

                    do tokens.pop_front();
                    while (parse_instruction(fn));

                    if (auto elsecond = std::get_if<token::word>(&tokens.front())) {
                        if (elsecond->val == "then") {
                            fn << then_label + ":";
                            return true;
                        }
                    } else {
                        error("Expected to find `then`", tokens.front());
                        return false;
                    }
                } else {
                    error("Expected to find `else` or `then`, found " + condition->val, tokens.front());
                }
            } else {
                error("Expected to find `else` or `then`", tokens.front());
                return false;
            }
        } else if (word->val == "else" || word->val == "then") {
            return false;
        } else if (word->val == "variable") {
            tokens.pop_front();
            if (auto var_name = std::get_if<token::word>(&tokens.front())) {
                fn.add_variable(var_name->val);
            } else {
                error("Expected variable name word, found other token.", tokens.front());
                return false;
            }
        } else if (fn.var_exists(word->val)) {
            fn << fn.get_var_ref(word->val);
        } else {
            fn << s::call(word->val);
        }
        return true;
    } else if (auto whole = std::get_if<token::whole>(&tokens.front())) {
        fn << s::push(whole->val);
        return true;
    } else if (auto string = std::get_if<token::string>(&tokens.front())) {
        std::string id = random_string();
        fn.strings[id] = string->val;

        fn << s::push("$" + fn.get_string(id));
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

    if (name_word->val == "main") {
        // Return the top of the stack
        fn << s::pop(s::rax);
    }

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
