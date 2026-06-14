/* STM32F407VG Linker Script */

ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
    RAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)

        _etext = .;
    } > FLASH

    _sidata = LOADADDR(.data);
    
    .data :
    {
        _sdata = .;

        *(.data)
        *(.data*)

        _edata = .;

    } > RAM AT > FLASH

    .bss :
    {
        _sbss = .;

        *(.bss)
        *(.bss*)
        *(COMMON)

        _ebss = .;

    } > RAM

    _end = .;
}
