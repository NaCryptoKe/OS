; boot.s - Kernel entry point

; Multiboot constants
MB_ALIGN	equ	1 << 0		; align loaded modules on page boundaries
MB_MEMINFO	equ	1 << 1		; provide memory map
MB_FLAGS	equ	MB_ALIGN | MB_MEMINFO
MB_MAGIC	equ	0x1BADB002	; 'magic number' lets the bootloader find the header
MB_CHECKSUM	equ	-(MB_MAGIC + MB_FLAGS)

section .multiboot
	align 4
	dd MB_MAGIC
	dd MB_FLAGS
	dd MB_CHECKSUM

section .bss
	align 16		; CPUs are picky so basically ensuring that data starts at a clean memory address
	stack_bottom:
		resb 16384	; 16 KiB for stack (1024 * 16) // resb means reserve byte
	stack_top:

section .text
	global _start
	extern kmain

_start:
	; Set up the stack pointer
	mov esp, stack_top

	; Call the C kernel entry point
	call kmain

	; If the kernel returns, just halt the CPU in a loop
	cli	; clear interrupts

.hang:
	hlt
	jmp .hang
