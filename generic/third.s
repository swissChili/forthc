.global third

# Duplicate the third item on the stack to the top of the stack
# ( a b c -- a b c a )
third:
    add $8, %rbp
    pop (%rbp)

    mov 16(%rsp), %rax
    push %rax

    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
