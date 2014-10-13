cmake_minimum_required(VERSION 2.8.4)
project(proj1)

set(SOURCE_FILES
    defs.h
    optional.h
    container_types.h
    array.c
    lexer_dfa.c
    lex.c
    Project1.c
    )
add_executable(lexer.bin ${SOURCE_FILES})
