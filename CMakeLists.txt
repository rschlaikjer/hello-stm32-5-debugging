cmake_minimum_required(VERSION 3.1.3)

# Set the ARM toolchain
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/toolchain-arm.cmake")
message(STATUS "Cmake toolchain: ${CMAKE_TOOLCHAIN_FILE}")

# Need to define a project right after the toolchain to force evaluation
project(stm32_with_opencm3_5 C CXX)

# Common paths
get_filename_component(LINKERSCRIPT_DIR "ldscripts" REALPATH)
get_filename_component(SCRIPTS_DIR "scripts" REALPATH)

# Include macro definitions
include(cmake/macros.cmake)

# Include submodule dependencies
add_subdirectory(submodules)

# Include common fault handler code
add_subdirectory(common)

# Global CFLAGS defaultsd
set(OPT "-Os")
set(DEBUG "-ggdb3")
set(CSTD "-std=c99")
set(CXXSTD "-std=c++14")

set(CFLAGS "-Wall -Wextra -Wshadow -Wimplicit-function-declaration \
            -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes \
            -fno-common -ffunction-sections -fdata-sections \
            -fno-builtin -fno-exceptions -fno-unwind-tables")

set(CXXFLAGS
    "-Wall -Wextra -Wshadow -Wredundant-decls -Weffc++ \
     -fno-common -ffunction-sections -fdata-sections -fno-threadsafe-statics \
     -fno-builtin -fno-rtti -fno-exceptions -fno-unwind-tables -fno-use-cxa-atexit")

# STM32F0 interrupt fault example
add_subdirectory(interrupt_fault_example)
