.global greaterthan

# a < b ?
greaterthan:
    add $8, %rbp
    pop (%rbp)

    pop %rax
    pop %rbx

    cmp %rax, %rbx
    jg .gt
    push $0
    jmp .done

.gt:
    push $1

.done:
    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
