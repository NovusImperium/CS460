cmake_minimum_required(VERSION 2.8.4)
project(proj2)

set(SOURCE_FILES
    defs.h
    dfa.c
    syn.c
    sym.c
    lex.c
    array.c
    hashmap.c
    SetLimits.c
    Project3.c
    )
add_executable(proj2 ${SOURCE_FILES})
