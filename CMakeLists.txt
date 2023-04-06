cmake_minimum_required(VERSION 3.9)
project(runit
        VERSION 1.4.0
        LANGUAGES C
        DESCRIPTION
        "The C test framework")


# -----------------------------------------------------------------------------
# Compiler flags
# -----------------------------------------------------------------------------
include(compiler_flags.cmake)

# -----------------------------------------------------------------------------
# Build targets
# -----------------------------------------------------------------------------
add_executable(${PROJECT_NAME}-selftest
        src/runit.h
        src/runit.c
        tst/selftest.c)

add_library(${PROJECT_NAME} src/runit.h src/runit.c)

target_include_directories(runit_selftest PRIVATE src/)
if (NOT MSVC)
    target_link_libraries(runit_selftest PRIVATE src/)
endif ()

enable_testing()
add_test(NAME ${PROJECT_NAME}-selftest COMMAND ${PROJECT_NAME}-selftest)

# Doxygen documentation builder
find_package(Doxygen OPTIONAL_COMPONENTS dot)
if (DOXYGEN_FOUND)
    # Cmake's wrapper of Doxygen, constructing a doxyfile from the
    # DOXYGEN_* variables, which are mapped to the Doxygen variables.

    # Output format
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN YES)
    set(DOXYGEN_SORT_MEMBER_DOCS NO)
    set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
    set(DOXYGEN_OUTPUT_LANGUAGE English)
    set(DOXYGEN_OUTPUT_DIRECTORY docs/)

    # Input format
    set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
    set(DOXYGEN_ALIASES license="**License:**")
    set(DOXYGEN_EXTRACT_ALL NO)
    set(DOXYGEN_INPUT_ENCODING UTF-8)

    # Input files
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
    set(DOXYGEN_EXCLUDE src/runit.c tst/)

    # Turn on all warnings, as errors
    set(DOXYGEN_WARNINGS YES)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
    set(DOXYGEN_WARN_IF_DOC_ERROR YES)
    set(DOXYGEN_WARN_IF_NO_PARAMDOC YES)
    set(DOXYGEN_WARN_AS_ERROR YES)

    # Parallelise
    set(DOXYGEN_NUM_PROC_THREADS 0)  # Auto set based on CPU

    # Graphviz's dot
    set(DOXYGEN_DOT_NUM_THREADS 0)  # Auto set based on CPU
    set(DOXYGEN_DOT_PATH)  # Empty path = search in $PATH

    # Generate command
    doxygen_add_docs(runit_doxygen
            src/runit.h LICENSE.md CHANGELOG.md README.md
            # List of input files for Doxygen
            )
else (DOXYGEN_FOUND)
    message(WARNING "Doxygen not found. Cannot generate documentation.")
endif (DOXYGEN_FOUND)
