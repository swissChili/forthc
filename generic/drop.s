.global drop

# Drop the top item from the stack. Just throw it away
drop:
    add $8, %rbp
    pop (%rbp)

    movq $0, (%rsp)
    add $8, %rsp

    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
