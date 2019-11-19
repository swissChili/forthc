.global swap


swap:
    mov 8(%rsp), %rax
    mov 16(%rsp), %rbx
    mov %rax, 16(%rsp)
    mov %rbx, 8(%rsp)

    ret
