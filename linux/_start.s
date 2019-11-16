.global _start
# expects main: to be defined
# returns the return value from main (%rax) to the kernel
_start:
    call main
    mov %rax, %rdi
    mov $60, %rax
    syscall
