.global puts

# Puts prints a null-terminated string to stdout
# Takes a pointer to a null terminated string on the stack and returns the length
# of the string (via strlen) on the stack.
puts:
    add $8, %rbp
    pop (%rbp)

    call strlen
    call writes
    pop %rax

    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
