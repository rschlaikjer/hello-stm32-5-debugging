MEMORY
{
    itcm (rwx) : ORIGIN = 0x00000000, LENGTH = 16K
    rom (rx) : ORIGIN = 0x08000000, LENGTH = 64K
    itcm_rom (rx) : ORIGIN = 0x00200000, LENGTH = 64K
    ram (rwx) : ORIGIN = 0x20000000, LENGTH = 320K
}

/* Include the common ld script. */
INCLUDE cortex-m-generic.ld

/* Place the itcm_code section in memory */
SECTIONS {
.itcm_code : {
    __itcm_code_start = .; /* VMA start */
    *(itcm_code)
    . = ALIGN(4);
    __itcm_code_end = .; /* VMA end */
} >itcm AT>rom
    __itcm_code_loadaddr = LOADADDR(.itcm_code);
}
