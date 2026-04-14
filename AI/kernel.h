#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

/* Defines the VGA text buffer address */
extern volatile unsigned char *VGA_BUFFER;

/* Function declarations for VGA text mode */
void putc(char c, int x, int y, unsigned char color);
void clear_screen(unsigned char color);

/* Main kernel entry point */
void kernel_main();

/* Structure to save CPU state on interrupt */
struct regs {
    uint32_t ds;                                    /* Data segment selector */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha */
    uint32_t int_no, err_code;                      /* Interrupt number and error code (if applicable) */
    uint32_t eip, cs, eflags, useresp, ss;          /* Pushed by the processor automatically */
};

#endif