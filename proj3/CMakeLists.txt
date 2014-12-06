cmake_minimum_required(VERSION 2.8.4)
project(proj3)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lm")
set(SOURCE_FILES
    defs.h
    dfa.c
    syn.c
    sym.c
    lex.c
    eval.c
    array.c
    hashmap.c
    set.c
    SetLimits.c
    Project3.c
    )
add_executable(proj3 ${SOURCE_FILES})

