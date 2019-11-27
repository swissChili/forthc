.global spaceship

# Implements the spaceship operator
# Works like strcmp() in C or <=> in other languages
# ( a b -- a b c )
# -1: a > b
# 0: a == b
# 1: a < b
spaceship:
    add $8, %rbp
    pop (%rbp)

    pop %rax
    pop %rbx

    cmp %rbx, %rax
    je spaceship.eq
    jg spaceship.gt

    push $-1
    jmp spaceship.done

spaceship.eq:
    push $0
    jmp spaceship.done

spaceship.gt:
    push $1

spaceship.done:
    mov (%rbp), %rbx
    sub $8, %rbp
    jmp *%rbx
