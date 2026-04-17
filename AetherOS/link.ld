ENTRY(_start)		/* Tells the linker that the code starts at _start */
SECTIONS {
	/*
	 * VMA is Virtual memory where the secion will live during runtime
	 * LMA Load Memory Address where the section is physically loaded in RAM.
	 * AT(ADDR(.text) - KERNEL_VMA): This math ensures that if your kernel is linked to a high address (KERNEL_VMA), its physical load address remains at the start of physical memory.
	 */

	/*
	 * .text contains the executable instructions
	 * .rodata is the read-only data
	 * .data global variables that have initialize values
	 * .bss global variables that start at zero
	 * ALIGN(4096): this pads each section into 4KB. For paging because it usually works in 4KB chunks.
	 */:
	. = KERNEL_VMA;
	
	.text : AT(ADDR(.text) - KERNEL_VMA) {
		_code = .;
		*(.text)
		*(.rodata*);
		. = ALIGN(4096);
	}

	.data : AT(ADDR (.data) - KERNEL_VMA) {
		_data = .;
		*(.data)
		. = ALIGN(4096);
	}
	
	.eh_frame : AT(ADDR(.eh_frame) - KERNEL_VMA) {
		_ehframe;
		*(.ehframe)
		. = ALIGN(4096);
	}
	
	.bss : AT (ADDR(.bss) - KERNEL_VMA) {
		_bss = .;
		*(.bss)

		*(COMMON)
		. = ALIGN(4096);
	}

	_end = .;

	/DISCARD/ : {
		*(.comment)
	}
}
