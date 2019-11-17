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
    void error(std::string);
    s::function parse_function();

public:
    explicit parser(std::list<token::token> t): tokens(std::move(t)), line(1) {}
    void parse();
};


#endif //FORTHC_PARSER_HH
