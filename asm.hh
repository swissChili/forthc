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
#endif

    std::string mov(std::string to, long num);
    std::string push(long num);
    std::string push(std::string reg);
    std::string pop(std::string to);
    std::string add(std::string a, std::string b);
    std::string call(std::string fn);

    class function {
    public:
        std::map<std::string, std::string> strings;
        std::string name;
        std::list<std::string> instructions;

        explicit function(std::string name) : name(std::move(name)) {}
        function &operator<<(std::string instruction);
        function &operator<<(std::list<std::string> insts);
        std::string assemble();
    };
}


#endif //FORTHC_ASM_HH
