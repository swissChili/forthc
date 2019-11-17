#ifndef FORTHC_PARSER_HH
#define FORTHC_PARSER_HH

#include "tokens.hh"
#include "asm.hh"
#include <list>
#include <utility>

namespace s = assembly;

class parser {
private:
    std::list<token::token> tokens;
    //unsigned index;
    unsigned line;
    void error(std::string, token::token &);
    s::function parse_function();
    bool parse_instruction(s::function &fn);
    std::map<std::string, std::list<std::string>> macro_words;

public:
    explicit parser(std::list<token::token> t, std::map<std::string, std::list<std::string>> macros)
    : tokens(std::move(t)), line(1), macro_words(std::move(macros)) {}
    std::list<s::function> parse();
};


#endif //FORTHC_PARSER_HH
