.global puti

# Puti prints an integer to stdout
# takes a 64 bit int from the stack
puti:
    add $8, %rbp
    pop (%rbp)

    # Local Variable (on callstack) for string
    add $20, %rbp

    # char *rsi = string_end;
    mov %rbp, %rsi
    # the value
    pop %rdi
    # rbx = 10
    mov $10, %rbx

puti.loop:
    # --rsi;
    dec %rsi
    # rax = rdi
    mov %rdi, %rax
    # rdx = 0
    xor %rdx, %rdx

    div %rbx

    # remainder in %rdx
    # rdx += '0'
    add $48, %rdx
    movb %dl, (%rsi)

    mov %rax, %rdi

    cmpb $0, (%rdi)
    je puti.done

    jmp puti.loop

puti.done:
    # Drop the local variable
    sub $20, %rbp

    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
