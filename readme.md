# forthc

This is a simple compiler for the Forth programming language. The only supported
architecture will be x64. I aim to support both Windows and POSIX systems, but
so far only Linux is supported.

This compiler compiles a dialect of Forth that I've dubbed nforth. It should be
familiar to any Forth programmer (not much is changed), but I've made some
changes to make the language more enjoyable for the average C programmer. Some
of these things involve making the string syntax sane, changing the way that
variable references are passed on the stack (which should only affect those
interested in modifying the compiler or writing native words anyway), and making
essentially everything a compile-only word. What that means is you can no longer
write something like this:

```forth
: square dup * ;
4 square .         \-> 16
```

Instead, you would have to write the only slightly less convenient

```forth
: square dup * ;
: main 4 square . ; \-> 16
```

This is just because it's a lot simpler to write a compiler if there is a set
entry point (main) rather than having to get all the top-level instructions and
throw them into a function.

## Programming in Forth

Forth, unlike C, Java, or whatever new language is in fashion this week, is a
stack-based language. This means that almost everything is done on the stack.
Here is an example of a function that returns the square of an integer in C and
in Forth. Note that the Forth code can be compiled by forthc even in it's
current state.

```c
int square ( int num )
{
    return num * num;
}
```

and in Forth:

```forth
: square dup * ;
```

The first thing you'll probably notice is that Forth doesn't have types. Like
Python (to an extent), it is dynamically typed. Unlike Python however, if you
mess up with the types, there won't be any type coersion and you'll probably
just segfault. I'd like to implement warnings in the compiler for doing things
you probably shouldn't be (like dereferencing an integer).

The code above defines a function named square with `: square'. The code until
the `;' is the body of the function. The first instruction (called a word in
forth) is `dup', which duplicates the top item on the stack (in this case, the
number to be squared). Then it calls `*' to multiply the top two items on the
stack and replace them with their product. Note that all subroutines in forth
share the same stack, so there is no need to explicitly return a value.

## building

Both the compiler and the standard library should be built with cmake. You will
need a C++17 compatible compiler and an assembler that uses AT&T syntax (Like
GAS). Cmake should detect these automatically. There are no library dependencies
other than the STL.

```sh
$ mkdir build && cd build
$ cmake ..
$ make
```

## Technical Details

There are a couple of things worth noting about the assembly that this compiler
produces. Most of these are a byproduct of the way forth functions, and the fact
that these paradigms don't translate perfectly to x86 machine code.

### Shared Stack

In Forth, all subroutines share the same stack. Arguments are passed on the
stack, calculations done on it, etc. There are, of course, named variables
(it is nearly impossible to do anything useful without them), but they are
used rarely. The issue with translating this to x86 is that by default, the
address a function should return to is stored on the stack. For instance,
consider the following Forth code:

```forth
: dbl dup + ;
: main 23 dbl ;
```

Literally translated to assembly this would look something like this:

```asm
dbl:
    pop %rax
    add %rax, %rax
    push %rax
    ret

main:
    push $23
    call dbl
    ret
```

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

```asm
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
```

This is a lot longer, and requires the runtime to allocate a second stack of
an appropriate size (which I haven't decided upon yet) but it seems like the
best option at the moment.
