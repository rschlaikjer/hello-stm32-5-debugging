cmake_minimum_required(VERSION 3.1.3)

# Opencm3
set(OPENCM3_DIR libopencm3)
get_filename_component(OPENCM3_DIR "${OPENCM3_DIR}/" REALPATH)
message(STATUS "OpenCM3 Directory: ${OPENCM3_DIR}")
include(ExternalProject)
ExternalProject_Add(libopencm3
    SOURCE_DIR "${OPENCM3_DIR}"
    BUILD_IN_SOURCE true
    DOWNLOAD_COMMAND ""
    CONFIGURE_COMMAND ""
    INSTALL_COMMAND ""
)
set(libopencm3_INCLUDE_DIRS
    ${OPENCM3_DIR}/include
    CACHE INTERNAL "Opencm3: Include Directories" FORCE)
set(libopencm3_LIB_DIRS
    ${OPENCM3_DIR}/lib
    CACHE INTERNAL "Opencm3: Include Directories" FORCE)

# Printf
set(PRINTF_DIR printf)
get_filename_component(PRINTF_DIR "${PRINTF_DIR}/" REALPATH)
message(STATUS "Printf Directory: ${PRINTF_DIR}")
set(printf_INCLUDE_DIRS
    "${PRINTF_DIR}"
    CACHE INTERNAL "printf: Include Directories" FORCE)
set(printf_SRCS
    "${PRINTF_DIR}/printf.c"
    CACHE INTERNAL "printf: Sources" FORCE)
