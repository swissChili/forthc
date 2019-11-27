.global lessthan

# a < b ?
lessthan:
    add $8, %rbp
    pop (%rbp)

    pop %rax
    pop %rbx

    cmp %rax, %rbx
    jl .lt
    push $0
    jmp .done

.lt:
    push $1

.done:
    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
