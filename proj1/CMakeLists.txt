cmake_minimum_required(VERSION 2.8.4)
project(proj1)

set(SOURCE_FILES
    lex.c
    dfa.c
    SetLimits.c
    Project1.c
    )
add_executable(proj1 ${SOURCE_FILES})