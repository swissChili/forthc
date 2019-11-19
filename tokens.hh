#ifndef TOKENS_HH
#define TOKENS_HH

#include <variant>
#include <string>


namespace token {
    struct word {
        std::string val;
        unsigned line;
    };

    struct whole {
        long val;
        unsigned line;
    };

    struct string {
        std::string val;
        unsigned line;
    };

    struct start_fn {
        unsigned line;
    };
    struct end_fn {
        unsigned line;
    };
    struct eof {
        unsigned line;
    };

    typedef std::variant<word, whole, start_fn, end_fn, eof, string> token;
}


#endif
