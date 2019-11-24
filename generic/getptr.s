.global getptr

getptr:
    add $8, %rbp
    pop (%rbp)

    pop %rax
    push (%rax)

    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
