cmake_minimum_required(VERSION 3.24)
project(runit
        VERSION 1.5.0
        LANGUAGES C
        DESCRIPTION
        "The C test framework")


add_library(${PROJECT_NAME} src/runit.c)
target_include_directories(
        ${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

if (NOT CMAKE_SYSTEM_NAME MATCHES "Generic")
    add_executable(${PROJECT_NAME}-selftest tst/selftest.c)
    target_link_libraries(${PROJECT_NAME}-selftest PRIVATE runit)

    enable_testing()
    add_test(NAME ${PROJECT_NAME}-selftest COMMAND ${PROJECT_NAME}-selftest)
endif ()


#################### FORMAT SECTION #######################
include(FetchContent)
FetchContent_Declare(
        rlibhelper
        GIT_REPOSITORY https://github.com/RoboticsHardwareSolutions/rlibhelper.git
        GIT_TAG main
)
FetchContent_MakeAvailable(rlibhelper)

set(FILES_FOR_FORMATTING
        src/runit.c
        src/runit.h
        tst/selftest.c
)
if (EXISTS "${rlibhelper_SOURCE_DIR}/format.cmake")
    include(${rlibhelper_SOURCE_DIR}/format.cmake)
else ()
    message(WARNING "format.cmake file not found")
endif ()
format_files(${PROJECT_NAME} SOURCES ${FILES_FOR_FORMATTING})