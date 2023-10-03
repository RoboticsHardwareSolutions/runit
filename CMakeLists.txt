cmake_minimum_required(VERSION 3.9)
project(runit
        VERSION 1.4.0
        LANGUAGES C
        DESCRIPTION
        "The C test framework")


# -----------------------------------------------------------------------------
# Compiler flags
# -----------------------------------------------------------------------------
#include(compiler_flags.cmake)

# -----------------------------------------------------------------------------
# Build targets
# -----------------------------------------------------------------------------
add_executable(${PROJECT_NAME}-selftest
        src/runit.h
        src/runit.c
        tst/selftest.c)

add_library(${PROJECT_NAME} src/runit.h src/runit.c)

target_include_directories(${PROJECT_NAME}-selftest PRIVATE src/)
if (NOT MSVC)
    target_link_libraries(${PROJECT_NAME}-selftest PRIVATE src/)
endif ()

enable_testing()
add_test(NAME ${PROJECT_NAME}-selftest COMMAND ${PROJECT_NAME}-selftest)

