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

std::string call_escape(std::string word) {
    std::string escaped;

    for (auto c : word) {
        switch (c) {
            case '-':
                escaped += "__DASH__";
                break;
            case '+':
                escaped += "__PLUS__";
                break;
            case '*':
                escaped += "__STAR__";
                break;
            case '/':
                escaped += "__SLASH__";
                break;
            case '^':
                escaped += "__CARET__";
                break;
            case '=':
                escaped += "__EQUALS__";
                break;
            case '<':
                escaped += "__LT__";
                break;
            case '>':
                escaped += "__GT__";
                break;
            case '.':
                escaped += "__DOT__";
                break;
            case ',':
                escaped += "__COMMA__";
                break;
            case '@':
                escaped += "__AT__";
                break;
            case '!':
                escaped += "__EXCLAMATION__";
                break;
            default:
                escaped.push_back(c);
        }
    }

    return escaped;
}

bool parser::parse_instruction(s::function &fn) {
    if (auto word = std::get_if<token::word>(&tokens.front())) {
        if (macro_words.find(word->val) != macro_words.end()) {
            fn << macro_words[word->val];
        } else if (word->val == "begin") {
            std::string loop_id = random_string();
            std::string cmp_label = "while_" + loop_id + "_begin";
            std::string end_label = "while_" + loop_id + "_end";

            fn << cmp_label + ":";

            do tokens.pop_front();
            while (parse_instruction(fn));

            // TODO: fix repeating code
            if (auto w = std::get_if<token::word>(&tokens.front())) {
                if (w->val != "while") {
                    error("Expected `while` found `" + w->val + "`", tokens.front());
                    return false;
                }
            } else {
                error("Expected `while` found other token", tokens.front());
                return false;
            }

            fn
                << s::pop(s::rax)
                << s::cmp(s::rax, "$0")
                << "\tje " + end_label;

            do tokens.pop_front();
            while (parse_instruction(fn));

            if (auto w = std::get_if<token::word>(&tokens.front())) {
                if (w->val != "repeat") {
                    error("Expected `repeat` found `" + w->val + "`", tokens.front());
                    return false;
                }
            } else {
                error("Expected `repeat` found other token", tokens.front());
                return false;
            }

            fn
                << "\tjmp " + cmp_label
                << end_label + ":";

        } else if (word->val == "do") {
            // ( limit index do ... loop )
            std::string loop_id = random_string();
            std::string limit_name = "loop_" + loop_id + "_limit";
            std::string index_name = "loop_" + loop_id + "_index";
            std::string start_label = fn.name + "_loop_start_" + loop_id;
            std::string end_label = fn.name + "_loop_end_" + loop_id;

            fn.add_variable(limit_name);
            fn.add_variable(index_name);

            fn
                << fn.get_var_ref_in(index_name, s::rax)
                << fn.get_var_ref_in(limit_name, s::rbx)
                << s::pop(s::deref(s::rax))
                << s::pop(s::deref(s::rbx))

                // Start the loop
                << start_label + ":"
                << s::mov(s::rcx, s::deref(s::rax))
                << s::mov(s::rdx, s::deref(s::rbx))
                << s::cmp(s::rdx, s::rcx)
                << "\tje " + end_label + "\n";

            do tokens.pop_front();
            while (parse_instruction(fn));

            fn
                << ""
                << fn.get_var_ref_in(index_name, s::rax)
                << fn.get_var_ref_in(limit_name, s::rbx)
                << "\tincq " + s::deref(s::rax);


            if (auto l = std::get_if<token::word>(&tokens.front())) {
                if (l->val == "loop") {
                    fn
                        << "\tjmp " + start_label
                        << end_label + ":";
                    return true;
                } else {
                    error("Expected `else` after do ..., found word " + l->val, tokens.front());
                    return false;
                }
            } else {
                error("Expected word after do ...", tokens.front());
                return false;
            }
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
        } else if (word->val == "else"
                || word->val == "then"
                || word->val == "loop"
                || word->val == "while"
                || word->val == "repeat") {
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
            fn << s::call(call_escape(word->val));
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

    s::function fn{call_escape(name_word->val)};

    bool is_main = name_word->val == "main";


    do if (!tokens.empty()) tokens.pop_front();
    while (parse_instruction(fn));

    if (is_main) {
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
