.global puts

puts:
    # ptr to string
    mov -8(%rsp), %rax
    # counter for length of string
    mov $0, %rbx

loop:
    cmpq $0, (%rax)
    je done

    inc %rax
    inc %rbx

    jmp loop

done:
    push %rbx
    call writes

    ret
