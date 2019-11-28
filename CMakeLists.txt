cmake_minimum_required(VERSION 3.13)
project(forthc)

set(CMAKE_CXX_STANDARD 17)
enable_language(CXX ASM)

add_executable(forthc main.cc tokens.hh lexer.cc lexer.hh debug.cc debug.hh parser.cc parser.hh asm.cc asm.hh random.cc random.hh colors.hh error.cc error.hh)
install(TARGETS forthc DESTINATION . OPTIONAL)
