cmake_minimum_required(VERSION 3.30)
project(assembler_project C)

set(CMAKE_C_STANDARD 17)

add_executable(assembler_project
        main.c
        HashTable/HashTable.c
        Pass1/Pass1.c
        Pass2/Pass2.c
)