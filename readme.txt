                               ~~ forthc ~~

This is a simple compiler for the Forth programming language. The only supported
architecture will be x64. I aim to support both Windows and POSIX systems, but
so far only Linux is supported.

                              ~~ building ~~

Both the compiler and the standard library should be built with cmake. You will
need a C++17 compatible compiler and an assembler that uses AT&T syntax (Like
GAS). Cmake should detect these automatically. There are no library dependencies
other than the STL.

    $ mkdir build && cd build
    $ cmake ..
    $ make

                          ~~ technical details ~~

There are a couple of things worth noting about the assembly that this compiler
produces. Most of these are a byproduct of the way forth functions, and the fact
that these paradigms don't translate perfectly to x86 machine code.

1. Shared Stack

    In Forth, all subroutines share the same stack. Arguments are passed on the
    stack, calculations done on it, etc. There are, of course, named variables
    (it is nearly impossible to do anything useful without them), but they are
    used rarely. The issue with translating this to x86 is that by default, the
    address a function should return to is stored on the stack. For instance,
    consider the following Forth code:

        : dbl dup + ;
        : main 23 dbl ;

    Literally translated to assembly this would look something like this:

        dbl:
            pop %rax
            add %rax, %rax
            push %rax
            ret

        main:
            push $23
            call dbl
            ret

    While this may seem correct on the surface anyone who has programmed in
    assembly should realize the problem. When double is called by main, the
    address at which it was called from is pushed to the stack. This is then
    used by double to return to where it was invoked. The issue is that double
    modifies the stack by returning on it. Also, the pop instruction would pop
    the return address, not the 23 passed as an argument.

    To fix this I have opted to have rbp point to a separate call stack, used to
    return from functions. When the function is called, the address of the
    caller is pushed to the call stack, and when that function wishes to return,
    it pops that address into a register and jumps to it. The correct code would
    look something like this:

        dbl:
            # [ 0 8 16 24 ]
            #       ^
            add $8, %rbp
            pop (%rbp)

            pop %rax
            add %rax, %rax
            push %rax

            mov (%rbp), %rax
            # [ 0 8 16 24 ]
            #     ^
            sub $8, %rbp
            jmp *%rax

        main:
            # [ 0 8 16 24 ]
            #     ^
            add $8, %rbp
            pop (%rbp)

            push $23
            call dbl

            mov (%rbp), %rbx
            # [ 0 8 16 24 ]
            #   ^
            sub $8, %rbp
            mov $0, %rax
            jmp *%rbx

    This is a lot longer, and requires the runtime to allocate a second stack of
    an appropriate size (which I haven't decided upon yet) but it seems like the
    best option at the moment.