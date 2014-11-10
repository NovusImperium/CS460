cmake_minimum_required(VERSION 2.8.4)
project(proj2)

set(SOURCE_FILES
    array.c
    defs.h
    parser.c
    lex.c
    dfa.c
    SetLimits.c
    Project2.c
    )
add_executable(proj2 ${SOURCE_FILES})

set(GENERATOR_FILES
    parser_gen.c
    )
add_executable(mkparser EXCLUDE_FROM_ALL ${GENERATOR_FILES})