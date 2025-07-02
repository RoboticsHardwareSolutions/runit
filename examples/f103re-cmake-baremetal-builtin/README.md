# Example for STM32F103RE

This example demonstrates a minimal bare-metal firmware project for the STM32F103RE microcontroller, using CMake for build configuration and targeting the ARM Cortex-M3 architecture. The project is structured to:

- **Integrate Unit Testing:** The runit unit testing framework is included and used to run a simple self-test at startup, verifying that no tests have failed initially.
- **RTT Debug Output:** SEGGER RTT is included for real-time debug output.
- **CMSIS Support:** The ARM CMSIS library is fetched and included for core support and device headers.
- **CMake Cross-Compilation:** The CMakeLists.txt sets up the ARM GCC toolchain, compiler flags, and links the necessary libraries and startup files.
- **Project Entry Point:** The main.c file runs self-tests, reports results, and then enters an infinite loop.

This setup is ideal for developing and testing embedded firmware with automated unit tests, without relying on an operating system or external test runner.
