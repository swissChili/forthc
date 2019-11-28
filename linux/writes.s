.global writes

# writes takes a pointer to a string and it's length as input
# and prints the string to stdout(1).
writes:
    add $8, %rbp
    pop (%rbp)
    # the ID of the syscall (sys_write)
    mov $1, %rax
    # The file descriptor for stdout
    mov $1, %rdi
    # length
    pop %rdx
    # buf
    pop %rsi
    syscall

    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
