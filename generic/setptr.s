.global setptr

# Dereference a pointer and move a value to it
# ( val ptr -- val ptr )
setptr:
    mov 8(%rsp), %rax
    mov 16(%rsp), %rbx
    mov %rbx, (%rax)
    ret
