#include "random.hh"

unsigned random_int() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);
    return dis(gen);
}

// Efficient? No. But I'm lazy.
std::string random_string() {
    unsigned length = 12;
    std::ostringstream s;

    for (int i = 0; i < length; i++) {
        s << std::to_string(random_int());
    }

    return s.str();
}
