#include <sstream>
#include "asm.hh"

namespace assembly {
    std::string mov(std::string inst, long num) {
#if ASSEMBLER == GAS
        return "\tmov " + inst + ", $" + std::to_string(num);
#endif
    }

    std::string push(long num) {
#if ASSEMBLER == GAS
        return "\tpush $" + std::to_string(num);
#endif
    }

    std::string push(std::string val) {
        return "\tpush " + val;
    }

    std::string call(std::string fn) {
        return "\tcall " + fn;
    }

    std::string pop(std::string to) {
        return "\tpop " + to;
    }

    std::string add(std::string a, std::string b) {
        return "\tadd " + a + ", " + b;
    }

    function &function::operator<<(std::string inst) {
        instructions.push_back(inst);
        // this might not be efficient, idk
        return *this;
    }

    function& function::operator<<(std::list<std::string> insts) {
        for (auto i : insts) {
            instructions.push_back(i);
        }
        return *this;
    }

    std::string function::assemble() {
        std::ostringstream code;

        code
            << "\n"
            << ".global " << name << "\n";

        for (auto const &[key, str] : strings) {
            code
                << name << "_str_" << key << ":\n"
                << "\t.string \"" << str << "\"\n";
        }

        code << name << ":\n";
        for (auto const &inst : instructions) {
            code << inst << "\n";
        }
        if (name == "main") {
            code << pop(rax) << "\n";
        }
        code << "\tret\n";

        return code.str();
    }
}
