# Set target to ARM cross-compile
message(STATUS "Processing toolchain file")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Prevent cmake from failing when testing the compiler works
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# CC/CXX executables
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# Use nano specs by default, global disable malloc
set(CMAKE_EXE_LINKER_FLAGS_INIT "--specs=nano.specs -specs=nosys.specs -Wl,--wrap=malloc")

# Locate the install dir for the arm-none-eabi toolchain
# This locates the version of libc our compiler would use
execute_process(
  COMMAND ${CMAKE_C_COMPILER} -print-file-name=libc.a
  OUTPUT_VARIABLE CMAKE_INSTALL_PREFIX
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Now get the directory that contains it
get_filename_component(CMAKE_INSTALL_PREFIX
    "${CMAKE_INSTALL_PREFIX}" DIRECTORY
)

# And then go up a dir
get_filename_component(CMAKE_INSTALL_PREFIX
  "${CMAKE_INSTALL_PREFIX}/.." REALPATH
)
set(CMAKE_INSTALL_PREFIX  ${CMAKE_INSTALL_PREFIX} CACHE FILEPATH
    "Install path prefix, prepended onto install directories.")

message(STATUS "Toolchain prefix: ${CMAKE_INSTALL_PREFIX}")

set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_OBJCOPY ${CMAKE_INSTALL_PREFIX}/bin/objcopy CACHE INTERNAL "objcopy tool")

set(CMAKE_FIND_ROOT_PATH ${CMAKE_INSTALL_PREFIX})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# GDB got renamed in Debian buster, figure out which one we have available
find_program(GDB_ARM NAMES gdb-multiarch arm-none-eabi-gdb)
message(STATUS "Found GDB: ${GDB_ARM}")
