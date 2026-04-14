ENTRY(_start)

SECTIONS
{
    . = 0x200000; /* Kernel starts at 2MB */

    .text :
    {
        *(.multiboot)
        *(.text)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
    }

    /* Define an end symbol for the kernel */
    _end = .;
}