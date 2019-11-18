.global puts

puts:
    add $8, %rbp
    pop (%rbp)

    # ptr to string
    mov (%rsp), %rax
    # counter for length of string
    mov $0, %rbx

puts.loop:
    cmpb $0, (%rax)
    je puts.done

    inc %rax
    inc %rbx

    jmp puts.loop

puts.done:
    push %rbx
    call writes

    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
