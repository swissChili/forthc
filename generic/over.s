.global over

over:
    add $8, %rbp
    pop (%rbp)

    mov 8(%rsp), %rax
    push %rax

    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
