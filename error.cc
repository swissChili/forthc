#include <iostream>
#include <fstream>
#include <iomanip>
#include "colors.hh"
#include "error.hh"

namespace err {
    void error(std::string f, unsigned line, int start, int current) {
        std::ifstream file(f);
        file.seekg(std::ios::beg);

        std::string buf;

        for (int i = 0; i < line; i++) {
            std::getline(file, buf);
        }

        std::cerr << std::setfill(' ') << std::setw(5) << line << " | ";
        std::cerr << buf << std::endl;

        for (int i = 1; i < start + 8; i++) std::cout << ' ';
        std::cout << C_RED << '^';
        for (int i = start; i < current - 1; i++) std::cout << '~';
        std::cout << C_RESET << std::endl;
    }
}