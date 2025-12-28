cmake_minimum_required(VERSION 4.1)

project(cpp-bare-metal LANGUAGES CXX ASM)

if(NOT CMAKE_SYSTEM STREQUAL "Generic" OR NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "riscv64")
    message(FATAL_ERROR "This project can only be built for freestanding riscv64")
endif()

include(cmake/settings.cmake)
include(cmake/create-program.cmake)

add_subdirectory(setup)

create_program(program.elf link.ld)
target_sources(program.elf PRIVATE src/main.cpp)

# The output should not be executable
