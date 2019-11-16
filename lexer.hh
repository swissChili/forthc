#ifndef FORTHC_LEXER_HH
#define FORTHC_LEXER_HH

#include <string>
#include <fstream>
#include <list>
#include "tokens.hh"

class lexer {
private:
    std::ifstream file;
    void error(std::string s);
    unsigned line;

public:
    lexer(std::string f);
    std::list<token::token> lex();
};


#endif //FORTHC_LEXER_HH
