#ifndef FORTHC_ASM_HH
#define FORTHC_ASM_HH

#include <list>
#include <map>
#include <string>
#include <utility>

#define ASSEMBLER GAS

namespace assembly {
#if ASSEMBLER == GAS
    static std::string rax = "%rax";
    static std::string rbx = "%rbx";
    static std::string rcx = "%rcx";
    static std::string rdx = "%rdx";
    static std::string r15 = "%r15";
    static std::string rbp = "%rbp";
#endif

    std::string mov(std::string to, long num);
    std::string push(long num);
    std::string push(std::string reg);
    std::string pop(std::string to);
    std::string add(std::string a, std::string b);
    std::string add(std::string a, long b);
    std::string mul(std::string a);
    std::string div(std::string a);
    std::string sub(std::string a, std::string b);
    std::string sub(std::string a, long b);
    std::string xor_(std::string a, std::string b);
    std::string call(std::string fn);
    std::string deref(const std::string& val);
    std::string mov(std::string t, std::string s);
    std::string cmp(std::string a, std::string b);

    class function {
    private:
        // A map of a variable name to its %rbp offset.
        // A variable should then be accessed with offset(%rbp)
        // %rbp isn't increased as variables are allocated so that they can be
        // referenced as they are defined. This means it is important to xor
        // each variable before the function exits
        std::map<std::string, unsigned short> variables;
        unsigned allocated_size;

    public:
        std::map<std::string, std::string> strings;
        std::string name;
        std::list<std::string> instructions;

        explicit function(std::string name) : name(std::move(name)), allocated_size(0) {}
        function &operator<<(std::string instruction);
        function &operator<<(std::list<std::string> insts);
        std::string get_string(std::string id);
        std::string assemble();
        void add_variable(std::string named);
        std::string get_variable(std::string named);
        std::list<std::string> get_var_ref(std::string named);
        std::list<std::string> get_var_ref_in(std::string named, std::string reg);
        bool var_exists(std::string named);
    };
}


#endif //FORTHC_ASM_HH
