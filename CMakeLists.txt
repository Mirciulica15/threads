cmake_minimum_required(VERSION 3.29)
project(untitled C)

set(CMAKE_C_STANDARD 99)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Werror)
endif()

add_executable(untitled main.c
        guessing-game.h
        guessing-game.c
        q-learning.c
        q-learning.h
        q-learning.h)
