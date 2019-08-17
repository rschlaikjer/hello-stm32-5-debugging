cmake_minimum_required(VERSION 3.1.3)

# Set the ARM toolchain
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/toolchain-arm.cmake")
message(STATUS "Cmake toolchain: ${CMAKE_TOOLCHAIN_FILE}")

# Need to define a project right after the toolchain to force evaluation
project(stm32_with_opencm3_5 C CXX)

# Common paths
get_filename_component(LINKERSCRIPT_DIR "ldscripts" REALPATH)
get_filename_component(SCRIPTS_DIR "scripts" REALPATH)

