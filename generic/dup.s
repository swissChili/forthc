.global dup

dup:
    add $8, %rbp
    pop (%rbp)

    mov (%rsp), %rax
    push %rax

    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
