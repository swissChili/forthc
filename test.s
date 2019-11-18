.global main

my_cool_str:
    .string "hello\n"

main:
    push $my_cool_str
    push $6
    call writes

    mov $0, %rax
    ret
