#include <sstream>
#include <algorithm>
#include "asm.hh"

namespace assembly {
    std::string mov(std::string inst, long num) {
#if ASSEMBLER == GAS
        return "\tmovq $" + std::to_string(num) + ", " + inst;
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
        return "\tadd $" + std::to_string(b) + ", " + a;
    }

    std::string xor_(std::string a, std::string b) {
        return "\txor " + b + ", " + a;
    }

    std::string mul(std::string a) {
        return "\tmul " + a;
    }

    std::string div(std::string a) {
        return "\tdiv " + a;
    }

    std::string sub(std::string a, std::string b) {
        return "\tsub " + b + ", " + a;
    }

    std::string sub(std::string a, long b) {
        return "\tsub $" + std::to_string(b) + ", " + a;
    }

    std::string deref(const std::string& val) {
        return "(" + val + ")";
    }

    std::string mov(std::string t, std::string s) {
        return "\tmov " + s + ", " + t;
    }

    std::string cmp(std::string a, std::string b) {
        return "\tcmp " + b + ", " + a;
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

    std::string function::get_string(std::string id) {
        return name + "_str_" + id;
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

        // Variable macros
        for (auto const &[key, val] : variables) {
            code << ".set " + name + "_var_" + key + ", -" + std::to_string(allocated_size - val) + "\n";
        }

        code
            << name << ":\n"
            << pop(deref(rbp)) << std::endl
            << add(rbp, allocated_size) << std::endl;

        for (auto const &inst : instructions) {
            code << inst << "\n";
        }

        code
            << sub(rbp, allocated_size) << std::endl
            << mov(rbx, deref(rbp)) << std::endl
            << "\tjmp *%rbx\n";

        return code.str();
    }

    void function::add_variable(const std::string& named) {
        variables[named] = allocated_size;
        // allocate another 64bit variable
        allocated_size += 8;
    }

    void function::add_variable(const std::string &named, unsigned sized) {
        variables[named] = allocated_size;
        allocated_size += sized;
    }

    bool function::var_exists(const std::string& named) {
        return variables.count(named) > 0
            || aliases[named].size() > 0;
    }

    std::string function::get_variable(const std::string& named) {
        return "$" + name + "_var_" + resolve_alias(named);
    }

    std::string function::resolve_alias(const std::string& named) {
        return aliases.count(named) ? aliases[named].front() : named;
    }

    std::list<std::string> function::get_var_ref(const std::string& named) {
        std::list<std::string> inst;

        inst.push_back(mov(rax, rbp));
        inst.push_back(add(rax, "$" + name + "_var_" + resolve_alias(named)));
        inst.push_back(push(rax));

        return inst;
    }

    void function::add_var_alias(const std::string& from, std::string to) {
        aliases[from].push_front(std::move(to));
    }

    void function::remove_var_alias(const std::string &named) {
        aliases[named].pop_front();
    }

    std::list<std::string> function::get_var_ref_in(const std::string& named,
                                                    const std::string& reg) {
        std::list<std::string> inst;
        inst.push_back(mov(reg, rbp));
        inst.push_back(add(reg, "$" + name + "_var_" + named));

        return inst;
    }
}
