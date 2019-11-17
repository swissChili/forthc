#ifndef FORTHC_LEXER_HH
#define FORTHC_LEXER_HH

#include <string>
#include <fstream>
#include <list>
#include "tokens.hh"

enum state {
    none,
    word,
    whole,
};

class lexer {
private:
    std::ifstream file;
    void error(std::string s);
    unsigned line;
    std::list<token::token> tokens;
    state s = none;
    void emplace_buf(std::string &buf);

public:
    lexer(std::string f);
    std::list<token::token> lex();
};


#endif //FORTHC_LEXER_HH
