.global strlen

# Calculate the length of a null-terminated string
strlen:
    add $8, %rbp
    pop (%rbp)

    mov (%rsp), %rax
    # Length starts at 1 (counting the \0)
    mov $1, %rbx

strlen.loop:
    cmpb $0, (%rax)
    je strlen.done

    inc %rax
    inc %rbx

    jmp strlen.loop

strlen.done:
    push %rbx

    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
