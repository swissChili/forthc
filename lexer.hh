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
    string,
    string_escape,
};

class lexer {
private:
    std::ifstream file;
    std::string filepath;
    void error(std::string s);
    unsigned line;
    std::list<token::token> tokens;
    state s = none;
    void emplace_buf(std::string &buf);
    std::string resolve_path(std::string relative);

public:
    lexer(std::string f);
    std::list<token::token> lex();
};


#endif //FORTHC_LEXER_HH
