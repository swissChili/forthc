.global puti


puti:
    add $8, %rbp
    pop (%rbp)

    pop %rax

    # Clear rdx
    xorl %edx, %edx
    # Move 10 to rbx
    movq $10, %rbx
    movq %rsp, %rcx
    decq %rcx
    xorq %r15, %r15
    # Sign extend and check if negative
    cmpl $0, %eax
    jge .printloop
    # So it's negative...
    negq %rax
    movq $1, %r15

.printloop:
    movq $10, %r14
    divq %r14
    addb $0x30, %dl
    movb %dl, (%rcx)
    xorl %edx, %edx
    # If the quotient is zero, stop
    cmpl $0, %eax
    je .doprint
    decq %rcx
    jmp .printloop

.doprint:
    # Handle if negative
    cmpq $0, %r15
    je .finishprint
    decq %rcx
    movb $'-', (%rcx)

.finishprint:
    movl $1, %eax
    movl $1, %edi
    leaq (%rcx), %rsi
    movq %rsp, %r10
    subq %rcx, %r10
    movq %r10, %rdx
    syscall

    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
