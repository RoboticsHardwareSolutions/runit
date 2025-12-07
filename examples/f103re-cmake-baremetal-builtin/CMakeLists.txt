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
project(example_f103re C CXX ASM)
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


################################ Clone neccessary dependencies ################################
set(FETCHCONTENT_QUIET FALSE) # To output process of FetchContent
include(FetchContent)

# CMSIS - Download required headers
set(CMSIS_BASE_URL "https://raw.githubusercontent.com/ARM-software/CMSIS_5/5.9.0/CMSIS/Core/Include")
set(CMSIS_HEADERS
  "core_cm3.h"
  "cmsis_version.h"
  "cmsis_compiler.h"
  "cmsis_gcc.h"
)

foreach(HEADER ${CMSIS_HEADERS})
  file(DOWNLOAD
    "${CMSIS_BASE_URL}/${HEADER}"
    "${CMAKE_BINARY_DIR}/cmsis/${HEADER}"
  )
endforeach()

include_directories(${CMAKE_BINARY_DIR}/cmsis)

# RTT
# The RTT library is used for debugging output.
FetchContent_Declare(
    rtt
    GIT_REPOSITORY https://github.com/SEGGERMicro/RTT.git
    GIT_TAG main
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
# Buffer size needs to be set to 16384 to avoid overflow in output. There is a lot of output in this example.
target_compile_definitions(rtt PUBLIC -DBUFFER_SIZE_UP=16384)

################################ Include Source Dir and cmsis includes ################################
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

################################ Add RUINT ################################
add_subdirectory(../../../runit "${CMAKE_CURRENT_BINARY_DIR}/runit")

################################ Create Target ################################
add_executable(${PROJECT_NAME}.elf "main.c" "syscalls.c" "sysinit.c" "startup_stm32f103xe.s" "STM32F103RETX_FLASH.ld")
target_link_libraries(${PROJECT_NAME}.elf PUBLIC rtt runit)

################################ Binary files ################################
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
