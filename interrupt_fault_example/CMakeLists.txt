cmake_minimum_required(VERSION 3.1.3)

enable_language(C CXX)

project(interrupt_fault_example C CXX)

set(LDSCRIPT STM32F091CBT6.ld)
set(CM3_FAMILY opencm3_stm32f0)
set(BMP_PORT "/dev/ttyACM0")

set(SRCS
    src/main.cpp
)

set(DEFS "${DEFS} ${USER_DEFS} -DSTM32F0 -DPRINTF_DISABLE_SUPPORT_FLOAT")
set(FP_FLAGS "${FP_FLAGS} -msoft-float")
set(ARCH_FLAGS "${ARCH_FLAGS} -mthumb -mcpu=cortex-m0 ${FP_FLAGS}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OPT} ${DEBUG} ${CSTD} ${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPT} ${DEBUG} ${CXXSTD} ${CXXFLAGS}")

set(LINKER_FLAGS
    "--static -nostartfiles \
    -T${LINKERSCRIPT_DIR}/${LDSCRIPT} ${ARCH_FLAGS} ${DEBUG} \
    -Wl,-Map=${PROJECT_NAME}.map -Wl,--cref \
    -Wl,--gc-sections -Wl,--no-wchar-size-warning \
    -Wl,--start-group -lc -lgcc -lstdc++ -lnosys -Wl,--end-group -lm")

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${libopencm3_INCLUDE_DIRS}
    ${printf_INCLUDE_DIRS}
    ${common_INCLUDE_DIRS}
)

link_directories(
    ${libopencm3_LIB_DIRS}
)

add_executable(interrupt_fault_example_elf
    ${SRCS}
    ${common_SOURCES}
    ${printf_SRCS}
)
add_dependencies(interrupt_fault_example_elf
    libopencm3
)
target_link_libraries(interrupt_fault_example_elf
    ${CM3_FAMILY}
)
target_compile_definitions(interrupt_fault_example_elf PRIVATE ${FLAGS})
set_target_properties(interrupt_fault_example_elf PROPERTIES COMPILE_FLAGS
    "${DEFS} ${ARCH_FLAGS}")
set_target_properties(interrupt_fault_example_elf PROPERTIES LINK_FLAGS
    "${LINKER_FLAGS}")
get_target_property(PROP interrupt_fault_example_elf COMPILE_FLAGS)

# Create additional targets for size / flashing
define_flash_target(interrupt_fault_example)
define_size_target(interrupt_fault_example)
include_gdbinit()
