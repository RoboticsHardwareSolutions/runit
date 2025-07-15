cmake_minimum_required(VERSION 3.24)
project(runit
        VERSION 1.5.0
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

add_library(${PROJECT_NAME} src/runit.c)
target_include_directories(
        ${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

if(NOT CMAKE_SYSTEM_NAME MATCHES "Generic")
        add_executable(${PROJECT_NAME}-selftest tst/selftest.c)
        target_link_libraries(${PROJECT_NAME}-selftest PRIVATE runit)

        enable_testing()
        add_test(NAME ${PROJECT_NAME}-selftest COMMAND ${PROJECT_NAME}-selftest)
endif()
