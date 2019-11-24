.global _start
.global _stack_top_addr
# expects main: to be defined
# returns the return value from main (%rax) to the kernel

_start:
    # Get the address to the beginning of the callstack using sys_brk(0)
    mov $12, %rax
    mov $0, %rdi
    syscall
    # Store that in %rbp
    mov %rax, %rbp

    # Allocate a callstack using sys_brk(4096)
    # Will allocate 512-calls worth of memory. After that will likely
    # just segfault but to be honest I don't know. Shouldn't be too hard to check
    lea 1024(%rax), %rdi
    mov $12, %rax
    syscall

    call main
    mov %rax, %rdi
    mov $60, %rax
    syscall
