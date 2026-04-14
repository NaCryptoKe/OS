#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

/* System call numbers */
#define SYS_EXIT    0
#define SYS_SEND    1
#define SYS_RECEIVE 2

/* Registers structure used for system calls (similar to interrupt frame) */
struct syscall_regs {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha or similar */
    uint32_t eip, cs, eflags, useresp, ss;          /* Pushed by the processor automatically */
};

/* Function to initialize the system call interface */
void init_syscalls();

/* System call handler in C */
void syscall_handler(struct syscall_regs *regs);

#endif