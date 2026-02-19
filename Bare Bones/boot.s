/* Declare constants for multiboot header */

/*.set is like #define in C,  */
/*Using Bit 0 is the "Align Modules" checkbox.
Bit 1 is the "Give me a Memory Map" checkbox. */
.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1            /* provide memory map */
/*The following are metadata the bootloader reads to understand  */
.set FLAGS,    ALIGN | MEMINFO  /* this is the Multiboot 'flag' field. Answers what it needs */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header. Tells if it's a kernel or not*/
/*If the magic number is changed the bootloader wouldn't even know if it was a kernel and will jump over it */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot. Checks if the data is valid or not */



/* 
Declare a multiboot header that marks the program as a kernel. These are magic values that are documented in the multiboot standard. The bootloader will search for this signature in the first 8 KiB of the kernel file, aligned at a 32-bit boundary. The signature is in its own section so the header can be forced to be within the first 8 KiB of the kernel file.
*/
.section .multiboot /*Keep this specific chunk of data separate from the rest of the code. Think of it as like the table of content in a book */
.align 4 /* ensures that the following data starts at an address that is a multiple of 4 */
/* Long is saying reserve 32-bits of spcace. */
.long MAGIC /* Stamp the 'Magic Number' here */
.long FLAGS      /* Stamp our 'Checkbox' choices here */
.long CHECKSUM   /* Stamp the 'Security Code' here */
/*
Offset,Data (Hex),What it represents
0x00,02 B0 AD 1B,The MAGIC number
0x04,03 00 00 00,"The FLAGS (e.g., 1"
0x08,FB 4F 52 E4,The CHECKSUM (The math that equals 0)

The bootloader starts at byte 0 and look for that 0x1BADB002 magic number. If they don't find it by byte 8,192 (8 KiB), they give up and assume your file is just a random cat picture or a text file, not a kernel.
*/


/*
The multiboot standard does not define the value of the stack pointer register(esp) and it is up to the kernel to provide a stack. This allocates room for a small stack by creating a symbol at the bottom of it, then allocating 16384 bytes (16KiB) for it, and finally creating a symbol at the top. The stack grows downwards on x86. The stack is in its own section so it can be marked nobits, which means the kernel file is smaller because it does not contain an uninitialized stack. The stack on x86 must be 16-byte aligned according to the System V ABI standard and de-facto extensions. The compiler will assume the stack is properly aligned and failure to align the stack will result in undefined behavior.
*/
.section .bss /* Reserves a space for the stack with out actually filling 16KiB worth of 0, it will only have a note in the header telling it */
.align 16 /* Is following the System V ABI because it says the stack pointer should be a multiple of 16 */


stack_bottom:
    .skip 16384 # 16 KiB    /*tells the assembler: "Move the 'pen' forward by 16,384 bytes." This creates the actual hole in memory for your data. */
stack_top:


/*
The linker script specifies _start as the entry point to the kernel and the bootloader will jump to this position once the kernel has been loaded. It doesn't make sense to return from this function as the bootloader is gone.
*/
.section .text
.global _start
.type _start, @function
_start:
    /*
    The bootloader has loaded us into 32-bit protected mode on a x86 machine. Interrupts are disabled. Paging is disabled. The processor state is as defined in the multiboot standard. The kernel has full control of the CPU. The kernel can only make use of hardware features and any code it provides as part of itself. There's no printf function, unless the kernel provides its own <stdio.h> header and a printf implementation. There are no security restrictions, no safeguards, no debugging mechanisms, only what the kernel provides itself. It has absolute and complete power over the machine.
    */

    /*
    To set up a stack, we set the esp register to point to the top of the stack (as it grows downwards on x86 systems). This is necessarily done in assembly as languages such as C cannot function without a stack.
    */
    mov $stack_top, %esp

	/*
	This is a good place to initialize crucial processor state before the high-level kernel is entered. It's best to minimize the early environment where crucial features are offline. Note that the processor is not fully initialized yet: Features such as floating point instructions and instruction set extensions are not initialized yet. The GDT should be loaded here. Paging should be enabled here. C++ features such as global constructors and exceptions will require runtime support to work as well.
	*/

	/*
	Enter the high-level kernel. The ABI requires the stack is 16-byte aligned at the time of the call instruction (which afterwards pushes the return pointer of size 4 bytes). The stack was originally 16-byte aligned above and we've pushed a multiple of 16 bytes to the stack since (pushed 0 bytes so far), so the alignment has thus been preserved and the call is well defined.
	*/
	call kernel_main

	/*
	If the system has nothing more to do, put the computer into an infinite loop. To do that:
	1) Disable interrupts with cli (clear interrupt enable in eflags).
    They are already disabled by the bootloader, so this is not needed. Mind that you might later enable interrupts and return from kernel_main (which is sort of nonsensical to do).
	2) Wait for the next interrupt to arrive with hlt (halt instruction).
    Since they are disabled, this will lock up the computer.
	3) Jump to the hlt instruction if it ever wakes up due to a non-maskable interrupt occurring or due to system management mode.
	*/
    cli      /* Clear Interrupts: Ignore all "pings" from hardware (keyboard, timer) */
1:  hlt      /* Halt: Tell the CPU to go to sleep until the next interrupt */
    jmp 1b   /* Jump 1-Back: If the CPU somehow wakes up, go back to 'hlt' */

/*
Set the size of the _start symbol to the current location '.' minus its start.
This is useful when debugging or when you implement call tracing.
*/
.size _start, . - _start    /* _start is where the code started, . is the address where we are, and gets the size from that */