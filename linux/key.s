.global key

# Read one key (byte) from stdin to the stack
key:
    add $8, %rbp
    pop (%rbp)

    # Make room for another byte
    pushq $0
    mov $0, %rax # sys_read
    mov $0, %rdi # stdin
    mov %rsp, %rsi # buffer
    mov $1, %rdx # count
    syscall

    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
