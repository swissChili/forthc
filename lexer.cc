#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include "lexer.hh"
#include "debug.hh"
#include "colors.hh"
#include "error.hh"

lexer::lexer(std::string f) {
    if (f != "-") {
        file.open(f, std::ifstream::in);
        filename = f;
        std::cerr << "Opened " << f << std::endl;
        int endsat = f.rfind('/');
        if (endsat == f.npos) {
            filepath = ".";
        } else {
            filepath = f.substr(0, endsat);
        }
    } else {
        file.open("/dev/stdin", std::ifstream::in);
        fromstdin = true;
        filepath = ".";
    }
    line = 1;
}

void lexer::error(std::string s) {
    std::cerr << "Error on line " << line << ": " << s << std::endl;

    if (!fromstdin) {
        err::error(filename, line, start, current);
    }

    throw err::lexing_error{};
}

void lexer::emplace_buf(std::string &buf) {
    if (s == word) {
        tokens.emplace_back(token::token{
            token::word{buf},
            line,
            filename,
            start,
            current
        });
    } else if (s == whole) {
        try {
            tokens.emplace_back(token::token{
                token::whole{std::stol(buf)},
                line,
                filename,
                start,
                current
            });
        } catch (std::invalid_argument) {
            error("Could not convert `" + buf + "` to long.");
        } catch (std::out_of_range) {
            error("Integer too large: `" + buf + "`");
        }
    }
    start = 0;
}

bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_num(char c) {
    return (c >= '0' && c <= '9');
}

bool is_reserved_safe(char c) {
    return c == '_'
        || c == '+'
        || c == '-'
        || c == '/'
        || c == '*'
        || c == '^'
        || c == '='
        || c == '<'
        || c == '>'
        || c == '.'
        || c == '!'
        || c == '@';
}

std::string lexer::resolve_path(std::string relative) {
    return filepath + "/" + relative;
}

std::string trim_leading(std::string source) {
    auto first = source.find_first_not_of(" \t");
    source.erase(0, first);
    return source;
}

std::list<token::token> lexer::lex() {
    char c;
    std::string buf;
    bool line_comment = false;
    bool await_include = false;
    unsigned comment_depth = 0;
    std::string str_prefix;
    start = 0;
    end = 0;
    current = 0;

    //std::cout << file.good();

    while ((c = file.get())) {
        current++;
        if (c == '(') {
            comment_depth++;
            continue;
        } else if (c == ')') {
            comment_depth--;
            continue;
        }

        if (s == string) {
            switch (c) {
                case '\n':
                    buf += "\\n";
                    line++;
                    current = 0;
                    start = 0;
                    end = 0;
                    break;
                case '\\':
                    s = string_escape;
                    break;
                case -1:
                    error("Expected string to end but reached end of file");
                    break;
                case '"':
                    tokens.emplace_back(token::token{
                        token::string{trim_leading(buf)},
                        line,
                        filename,
                        start,
                        current
                    });
                    if (!str_prefix.empty()) {
                        tokens.emplace_back(token::token{
                            token::word{str_prefix},
                            line,
                            filename,
                            start,
                            static_cast<int>(start + str_prefix.length())
                        });
                        str_prefix = "";
                    }
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
        } else if (comment_depth > 0) {
            // Ignore block comments
            continue;
        }

        if (c == '\n') {
            line++;
            current = 0;
            start = 0;
            end = 0;

            if (line_comment) {
                line_comment = false;
            }
        } else if (c == -1) {
            tokens.emplace_back(token::token{
                token::eof{},
                line
            });
            return tokens;
        }

        if (line_comment) {
            continue;
        }

        if (is_alpha(c) || is_reserved_safe(c)) {
            if (await_include)
                error("nforth requires that included filenames be quoted.");

            if (!start) start = current;

            buf.push_back(c);
            s = word;
        } else if (c == '\\') {
            line_comment = true;
        } else if (is_num(c) || c == '-') {
            if (await_include)
                error("nforth requires that included filenames be quoted.");

            if (!start) start = current;

            buf.push_back(c);
            s = whole;
        } else if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            if (await_include) {
                // Remove the string
                tokens.pop_back();

                lexer included{resolve_path(buf)};
                auto included_tokens = included.lex();

                for (auto t : included_tokens) {
                    if (!std::get_if<token::eof>(&t.node))
                        tokens.push_back(std::move(t));
                }
                await_include = false;
                s = none;
                buf = "";
            } else if (buf == "include") {
                await_include = true;
                s = none;
                buf = "";
            } else {
                emplace_buf(buf);

                s = none;
                buf = "";
            }
        } else if (c == ':') {
            emplace_buf(buf);

            tokens.emplace_back(token::token{
                token::start_fn{}, line, filename, current, current
            });
            start = 0;
            s = none;
            buf = "";
        } else if (c == ';') {
            emplace_buf(buf);

            tokens.emplace_back(token::token{
                token::end_fn{}, line, filename, current, current
            });
            s = none;
            buf = "";
        } else if (c == '"') {
            if (s == none) {
                start = current;
                s = string;
            } else if (s == string_escape) {
                buf += "\\\"";
                s = string;
            } else if (s == word) {
                str_prefix = buf + "-string";
                s = string;
                buf = "";
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
