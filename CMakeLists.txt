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

add_library(${PROJECT_NAME} src/runit.c)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        )


add_executable(${PROJECT_NAME}-selftest
        tst/selftest.c)

target_link_libraries(runit-selftest PRIVATE runit)

target_include_directories(${PROJECT_NAME}-selftest
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        )

enable_testing()
add_test(NAME ${PROJECT_NAME}-selftest COMMAND ${PROJECT_NAME}-selftest)

