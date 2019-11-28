.macro startfn
    add $8, %rbp
    pop (%rbp)
.endm

.macro endfn
    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
.endm
