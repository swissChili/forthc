#ifndef FORTHC_ASM_H
#define FORTHC_ASM_H


#define BINARY_OP(name) \
           "\tpop\t%rax\n" \
        << "\tpop\t%rbx\n" \
        << "\t" #name "\t%rax, %rbx\n" \
        << "\tpush\t%rax\n"

#define RAX_OP(name) \
           "\tpop\t%rbx\n" \
        << "\tpop\t%rax\n" \
        << "\t" #name "\t%rbx\n" \
        << "\tpush\t%rax\n"

#endif
