################################ CrossCopiler ################################
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.28)

# specify cross compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(robocontroller C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

################################ Compiler options ################################
add_compile_definitions(ARM_MATH_CM3;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
#add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
#add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-Wl,-gc-sections,--print-memory-usage)
add_link_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_link_options(-T ${CMAKE_SOURCE_DIR}/STM32F103RETX_FLASH.ld)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-O3 -g)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-O3 -g)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os -g)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "MinOptDebInfo")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Og -g)
else()
    add_compile_options(-g)
    message(STATUS "No optimization")
endif()


################################ Clone neccessary repos ################################
set(FETCHCONTENT_QUIET FALSE) # To output process of FetchContent

# CMSIS
include(FetchContent)
FetchContent_Declare(
    cmsis
    GIT_REPOSITORY https://github.com/ARM-software/CMSIS_5
    GIT_TAG master
)
FetchContent_MakeAvailable(cmsis)

# RTT
include(FetchContent)
FetchContent_Declare(
    rtt
    GIT_REPOSITORY https://github.com/SEGGERMicro/RTT.git
    GIT_TAG master
)
FetchContent_MakeAvailable(rtt)

################################ Create RTT Lib ################################
add_library(rtt STATIC
    ${rtt_SOURCE_DIR}/RTT/SEGGER_RTT_ASM_ARMv7M.S
    ${rtt_SOURCE_DIR}/RTT/SEGGER_RTT_printf.c
    ${rtt_SOURCE_DIR}/RTT/SEGGER_RTT.c
)
target_include_directories(rtt
    PUBLIC
    $<BUILD_INTERFACE:${rtt_SOURCE_DIR}/RTT>
    $<BUILD_INTERFACE:${rtt_SOURCE_DIR}/Config>
)

################################ Include Source Dir and cmsis includes ################################
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${cmsis_SOURCE_DIR}/CMSIS/Core/Include
)

################################ Add RUINT ################################
add_subdirectory(../../../runit "${CMAKE_CURRENT_BINARY_DIR}/runit")

################################ Create Target ################################
add_executable(${PROJECT_NAME}.elf "main.c" "syscalls.c" "sysinit.c" "startup_stm32f103xe.s" "STM32F103RETX_FLASH.ld")
target_link_libraries(${PROJECT_NAME}.elf PUBLIC rtt runit)
