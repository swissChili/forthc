#include <iostream>
#include <fstream>
#include "lexer.hh"

lexer::lexer(std::string f) {
    if (f != "-") {
        file.open(f, std::ifstream::in);
        std::cerr << "Opened " << f << std::endl;
    } else {
        file.open("/dev/stdin", std::ifstream::in);
    }
    line = 0;
}

void lexer::error(std::string s) {
    std::cerr << "Error on line " << line + 1 << ": " << s << std::endl;
    throw s;
}

void lexer::emplace_buf(std::string &buf) {
    if (s == word) {
        tokens.emplace_back(token::word{buf,line});
    } else if (s == whole) {
        try {
            tokens.emplace_back(token::whole{std::stol(buf), line});
        } catch (std::invalid_argument) {
            error("Could not convert `" + buf + "` to long.");
        } catch (std::out_of_range) {
            error("Integer too large: `" + buf + "`");
        }
    }
}

std::list<token::token> lexer::lex() {
    char c;
    std::string buf;
    bool line_comment = false;

    //std::cout << file.good();

    while ((c = file.get())) {
        // std::cout << c << line_comment << std::endl;

        if (s == string) {
            switch (c) {
                case '\n':
                    buf += "\\n";
                    line++;
                    break;
                case '\\':
                    s = string_escape;
                    break;
                case -1:
                    error("Expected string to end but reached end of file");
                    break;
                case '"':
                    tokens.emplace_back(token::string{buf, line});
                    s = none;
                    break;
                default:
                    buf.push_back(c);
            }
            continue;
        } else if (s == string_escape) {
            switch (c) {
                case 'n':
                    buf += "\\n";
                    break;
                case 't':
                    buf.push_back('\t');
                    break;
                case '\\':
                    buf += "\\\\";
                    break;
                case '"':
                    buf += "\\\"";
                    break;
                default:
                    error("Invalid escape string: \\" + std::to_string(c));
                    break;
            }
            s = string;
            continue;
        }

        if (c == '\n') {
            line++;

            if (line_comment) {
                line_comment = false;
            }
        } else if (c == -1) {
            tokens.emplace_back(token::eof{line});
            return tokens;
        }

        if (line_comment) {
            continue;
        }

        if ((c >= 'a' && c <= 'z')
         || (c >= 'A' && c <= 'Z')
         || c == '_'
         || c == '+'
         || c == '-'
         || c == '/'
         || c == '*'
         || c == '^'
         || c == '='
         || c == '<'
         || c == '>') {
            buf.push_back(c);
            s = word;
        } else if (c == '\\') {
            line_comment = true;
        } else if ((c >= '0' && c <= '9')) {
            buf.push_back(c);
            s = whole;
        } else if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            emplace_buf(buf);

            s = none;
            buf = "";
        } else if (c == ':') {
            emplace_buf(buf);

            tokens.emplace_back(token::start_fn{line});
            s = none;
            buf = "";
        } else if (c == ';') {
            emplace_buf(buf);

            tokens.emplace_back(token::end_fn{line});
            s = none;
            buf = "";
        } else if (c == '"') {
            if (s == none) {
                s = string;
            } else if (s == string_escape) {
                buf.push_back(c);
                s = string;
            } else {
                emplace_buf(buf);
                s = string;
                buf = "";
            }
        } else {
            error("Unexpected char `" + std::to_string(c) + "`");
        }
    }

    return tokens;
}
