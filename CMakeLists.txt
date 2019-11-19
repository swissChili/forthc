cmake_minimum_required(VERSION 3.13)
project(forthc)

set(CMAKE_CXX_STANDARD 17)
enable_language(CXX ASM)

add_executable(forthc main.cc tokens.hh lexer.cc lexer.hh debug.cc debug.hh parser.cc parser.hh asm.cc asm.hh random.cc random.hh)
add_library(fstd STATIC
        linux/_start.s
        linux/puts.s
        linux/writes.s
        linux/strlen.s
        linux/streq.s
        linux/puti.s
        generic/swap.s
        generic/dup.s
        generic/drop.s
        generic/tuck.s
        generic/over.s)
