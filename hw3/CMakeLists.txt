cmake_minimum_required(VERSION 2.8.4)
project(hw3)

set(COMMON_SOURCES
    defs.h
    fraction.c
    gcd.c
    )

set(MAIN_SOURCES
    ${COMMON_SOURCES}
    heap.c
    gillH03.c
    )
add_executable(H03.out ${MAIN_SOURCES})

add_executable(main.debug EXCLUDE_FROM_ALL ${MAIN_SOURCES})
set_target_properties(
    main.debug
    PROPERTIES
    COMPILE_FLAGS "${COMPILE_FLAGS} -g -fno-omit-frame-pointer"
    LINK_FLAGS "-lprofiler"
    )

set(HEAP_SOURCES
    ${COMMON_SOURCES}
    heap.c
    heap_main.c
    )
add_executable(heap.bin EXCLUDE_FROM_ALL ${HEAP_SOURCES})
add_executable(heap.debug EXCLUDE_FROM_ALL ${HEAP_SOURCES})
set_target_properties(
    heap.debug
    PROPERTIES
    COMPILE_FLAGS "${COMPILE_FLAGS} -g -fno-omit-frame-pointer"
    LINK_FLAGS "-lprofiler"
    )

set(BST_SOURCES
    ${COMMON_SOURCES}
    bst.c
    bst_main.c
    )
add_executable(bst.bin EXCLUDE_FROM_ALL ${BST_SOURCES})
add_executable(bst.debug EXCLUDE_FROM_ALL ${BST_SOURCES})
set_target_properties(
    bst.debug
    PROPERTIES
    COMPILE_FLAGS "${COMPILE_FLAGS} -g -fno-omit-frame-pointer"
    LINK_FLAGS "-lprofiler"
    )

set(MERGE_SOURCES
    ${COMMON_SOURCES}
    farray.c
    merge_main.c
    )
add_executable(merge.bin EXCLUDE_FROM_ALL ${MERGE_SOURCES})
add_executable(merge.debug EXCLUDE_FROM_ALL ${MERGE_SOURCES})
set_target_properties(
    merge.debug
    PROPERTIES
    COMPILE_FLAGS "${COMPILE_FLAGS} -g -fno-omit-frame-pointer"
    LINK_FLAGS "-lprofiler"
    )

set(INPUT_SOURCES
    mkinput.cpp
    )
add_executable(mkinput.bin EXCLUDE_FROM_ALL ${INPUT_SOURCES})

set(HEAP_TEST_SOURCES
    heap_test.c
    heap_test_main.c
    )
add_executable(heap_test.bin EXCLUDE_FROM_ALL ${HEAP_TEST_SOURCES})
