cmake_minimum_required(VERSION 2.8.4)
project(proj1)

set(SOURCE_FILES
    defs.h
    array.c
    lexer_dfa.c
    lexer.c
    lex.c
    proj1.c
    )
add_executable(lexer.bin ${SOURCE_FILES})
