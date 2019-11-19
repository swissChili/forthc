.global main


epic_str:
    .string "Hello World\n"

dbl:
    # [ 0 8 16 24 ]
    #       ^
    add $8, %rbp
    pop (%rbp)

    pop %rax
    add %rax, %rax
    push %rax

    mov (%rbp), %rbx
    # [ 0 8 16 24 ]
    #     ^
    sub $8, %rbp
    jmp *%rbx

main:
    # [ 0 8 16 24 ]
    #     ^
    add $8, %rbp
    pop (%rbp)

    #push $23
    #call dbl

    push $epic_str
    call puti

    xor %rax, %rax

    mov (%rbp), %rbx
    # [ 0 8 16 24 ]
    #   ^
    sub $8, %rbp
    jmp *%rbx
