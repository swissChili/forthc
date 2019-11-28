#ifndef TOKENS_HH
#define TOKENS_HH

#include <variant>
#include <string>


namespace token {
    struct word {
        std::string val;
    };

    struct whole {
        long val;
    };

    struct string {
        std::string val;
    };

    struct start_fn {
    };
    struct end_fn {
    };
    struct eof {
    };

    typedef std::variant<word, whole, start_fn, end_fn, eof, string> node_t;

    struct token {
        node_t node;
        unsigned line;
        int starts_at;
        int ends_at;
    };
}


#endif
