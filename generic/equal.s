.global equal

# Pushes 1 onto the stack of the top two stack items are equal
# otherwise pushes 0
equal:
    add $8, %rbp
    pop (%rbp)

    pop %rax
    pop %rbx

    cmp %rax, %rbx

    jne equal.ne

    pushq $1
    jmp equal.done

equal.ne:
    pushq $0

equal.done:
    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
