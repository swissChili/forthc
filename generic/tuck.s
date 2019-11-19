.global tuck

# Tuck essentially swaps and dupplicates
# 1 2 3 tuck \ 1 3 2 3
tuck:
    add $8, %rbp
    pop (%rbp)

    mov (%rsp), %rax
    call swap
    push %rax

    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
