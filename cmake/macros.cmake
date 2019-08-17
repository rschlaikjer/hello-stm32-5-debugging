# Create a target that prints the size of the text/data/bss sections in an ELF
# file. The target ${BASE_TARGET_elf} must exist.
macro(define_size_target BASE_TARGET)
add_custom_target(
    ${BASE_TARGET}_size ALL
    DEPENDS ${BASE_TARGET}_elf
    COMMAND echo "section      size   addr"
    COMMAND arm-none-eabi-size -A -d ${BASE_TARGET}_elf | egrep 'text|data|bss' | numfmt --field=2 --to=iec-i --suffix=B | awk '{printf(\"%10s %10s   0x%08x\\n\",$$1,$$2,$$3)}'
)
endmacro(define_size_target)

# Create a target that uploads an ELF.
# The target ${BASE_TARGET_elf} must exist, and ${BMP_PORT} must be defined.
macro(define_flash_target BASE_TARGET)
add_custom_target(
    ${BASE_TARGET}_flash
    DEPENDS ${SCRIPTS_DIR}/bmp_flash.scr ${BASE_TARGET}_elf
    COMMAND ${GDB_ARM} --batch -ex 'target extended-remote ${BMP_PORT}' -x ${SCRIPTS_DIR}/bmp_flash.scr ${BASE_TARGET}_elf
)
endmacro(define_flash_target)

# Create rule to copy gdbinit file to build dir
macro(include_gdbinit)
configure_file(${SCRIPTS_DIR}/gdbinit ${CMAKE_CURRENT_BINARY_DIR}/.gdbinit COPYONLY)
endmacro(include_gdbinit)
