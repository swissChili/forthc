.global writes

# writes takes a pointer to a string and it's length as input
# and prints the string to stdout(1).
writes:
    # the ID of the syscall (sys_write)
    mov $1, %rax
    # The file descriptor for stdout
    mov $1, %rdi
    # length
    mov 8(%rsp), %rdx
    # buf
    mov 16(%rsp), %rsi
    syscall

    ret
