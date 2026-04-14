.section .multiboot
.align 4
.long 0x1BADB002         /* Multiboot magic number */
.long 0x00000000         /* Flags - align modules on page boundaries, enable A20 */
.long -(0x1BADB002 + 0x00000000) /* Checksum */

.section .text
.global _start
_start:
    /* Set up stack */
    mov $stack_top, %esp

    /* Call kernel_main (defined in C) */
    call kernel_main

    /* Loop forever if kernel_main returns */
    cli
halt:
    hlt
    jmp halt

.section .bss
.align 4096
stack_bottom:
.skip 16384 /* 16 KiB stack */
stack_top:
