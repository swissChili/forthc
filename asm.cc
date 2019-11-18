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
        return "\tadd " + b + ", " + a;
    }

    std::string add(std::string a, long b) {
        return "\tadd " + std::to_string(b) + "," + a;
    }

    std::string sub(std::string a, std::string b) {
        return "\tsub " + b + ", " + a;
    }

    std::string sub(std::string a, long b) {
        return "\tsub " + std::to_string(b) + "," + a;
    }

    std::string deref(const std::string& val) {
        return "(" + val + ")";
    }

    std::string mov(std::string t, std::string s) {
        return "\tmov " + s + ", " + t;
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

        code
            << name << ":\n"
            << add(rbp, 8) << std::endl
            << pop(deref(rbp)) << std::endl;

        for (auto const &inst : instructions) {
            code << inst << "\n";
        }

        code
            << mov(rbx, deref(rbp)) << std::endl
            << sub(rbp, 8) << std::endl
            << "\tjmp *%rbx\n";

        return code.str();
    }
}
