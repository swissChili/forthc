                        ~~ forthc ~~

This is a simple compiler for the Forth programming language.
The only supported architecture will be x64. I aim to support
both Windows and POSIX systems, but so far only Linux is
supported.

                       ~~ building ~~

Both the compiler and the standard library should be built with
cmake. You will need a C++17 compatible compiler and an
assembler that uses AT&T syntax (Like GAS). Cmake should detect
these automatically. There are no library dependencies other
than the STL.

    $ mkdir build && cd build
    $ cmake ..
    $ make

