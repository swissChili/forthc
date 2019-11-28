#ifndef FORTHC_ERROR_HH
#define FORTHC_ERROR_HH

#include <string>
#include <exception>

namespace err {
    void error(std::string file, unsigned line, int start, int end);

    struct parsing_error : std::exception {};
    struct lexing_error : std::exception {};
}


#endif //FORTHC_ERROR_HH
