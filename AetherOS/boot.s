.set FLAGS, 0x00
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss		/*.bss is block started by symbol*/
.align 16

stack_bottom:
	.skip 16384	/* 16 KiB*/

stack_top:
	.section .text
	.global _start
	.type _start, @function
	_start:			/* after the bootloader us into 32-bit*/
		mov $stacktop, %esp	/* Telling the CPU that the stack starts at stacktop*/
		call kernel_main

		cli		/* Clear the inturrupt*/
		1: hlt		/* Wait for the next inturrupt*/
		jmp 1b		/* jump to hlt if a non maskable inturrupt occurs*/

.size _start, . - _start
