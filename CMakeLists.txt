cmake_minimum_required(VERSION 3.13)
project(forthc)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

enable_language(C CXX ASM)

set(CMAKE_CXX_STANDARD 14)


flex_target(lexer forth.l "${CMAKE_CURRENT_BINARY_DIR}/F.cc")

bison_target(parser forth.y "${CMAKE_CURRENT_BINARY_DIR}/B.cc")


add_executable(forthc
    "${CMAKE_CURRENT_BINARY_DIR}/F.cc"
    "${CMAKE_CURRENT_BINARY_DIR}/B.cc"
    "${CMAKE_SOURCE_DIR}/asm.h"
    )

target_include_directories(forthc PUBLIC .)


add_library(fstd
    "${CMAKE_SOURCE_DIR}/linux/_start.s")
