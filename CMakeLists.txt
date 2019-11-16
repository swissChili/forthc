project(forthc)

set(CMAKE_CXX_STANDARD 17)

add_executable(forthc main.cc tokens.hh lexer.cc lexer.hh debug.cc debug.hh)
