.global streq

# Check if two null-terminated strings are equal
# Takes two string pointers on the stack, returns $1 if they are equal $0 else
streq:
    add $8, %rbp
    pop (%rbp)

    mov (%rsp), %rax
    mov 8(%rsp), %rbx
    # the return value
    mov $1, %rdi

streq.loop:
    mov (%rax), %dx
    cmpb %dl, (%rbx)
    jne streq.ne

    # String is over (\0)
    cmpb $0, %al
    je streq.done

    inc %rax
    inc %rbx

    jmp streq.loop

streq.ne:
    mov $0, %rdi

streq.done:
    push %rdi

    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
