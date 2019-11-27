.global getbyteptr

getbyteptr:
    add $8, %rbp
    pop (%rbp)

    pop %rax
    movb (%rax), %bl
    push %rbx

    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
