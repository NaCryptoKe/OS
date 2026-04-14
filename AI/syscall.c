#include "syscall.h"
#include "kernel.h"
#include "task.h" /* For current_task */
#include "ipc.h"  /* For send_message, receive_message */

/* Current task, defined in task.c */
extern task_t *current_task;

/* System call handler */
void syscall_handler(struct syscall_regs *regs) {
    /* The system call number is in EAX */
    uint32_t syscall_num = regs->eax;

    switch (syscall_num) {
        case SYS_EXIT: {
            putc('S', 0, 5, 0x0F);
            putc('Y', 1, 5, 0x0F);
            putc('S', 2, 5, 0x0F);
            putc('_', 3, 5, 0x0F);
            putc('E', 4, 5, 0x0F);
            putc('X', 5, 5, 0x0F);
            putc('I', 6, 5, 0x0F);
            putc('T', 7, 5, 0x0F);
            /* For now, just mark the current task as terminated */
            current_task->state = TASK_TERMINATED;
            switch_task();
            break;
        }
        case SYS_SEND: {
            /* dest_pid in EBX, message_t* in ECX */
            uint32_t dest_pid = regs->ebx;
            message_t *msg = (message_t*)regs->ecx;
            send_message(dest_pid, msg);
            break;
        }
        case SYS_RECEIVE: {
            /* source_pid* in EBX, message_t* in ECX */
            uint32_t *source_pid = (uint32_t*)regs->ebx;
            message_t *msg = (message_t*)regs->ecx;
            receive_message(source_pid, msg);
            break;
        }
        default: {
            putc('U', 0, 5, 0x0F);
            putc('n', 1, 5, 0x0F);
            putc('k', 2, 5, 0x0F);
            putc('n', 3, 5, 0x0F);
            putc('o', 4, 5, 0x0F);
            putc('w', 5, 5, 0x0F);
            putc('n', 6, 5, 0x0F);
            putc(' ', 7, 5, 0x0F);
            putc('S', 8, 5, 0x0F);
            putc('Y', 9, 5, 0x0F);
            putc('S', 10, 5, 0x0F);
            putc('C', 11, 5, 0x0F);
            putc('A', 12, 5, 0x0F);
            putc('L', 13, 5, 0x0F);
            putc('L', 14, 5, 0x0F);
            /* For now, halt the system */
            asm volatile("cli; hlt");
            break;
        }
    }
}

void init_syscalls() {
    /* No special initialization needed beyond setting the IDT entry, which is done in idt.c */
    putc('S', 0, 4, 0x0F);
    putc('y', 1, 4, 0x0F);
    putc('s', 2, 4, 0x0F);
    putc('c', 3, 4, 0x0F);
    putc('a', 4, 4, 0x0F);
    putc('l', 5, 4, 0x0F);
    putc('l', 6, 4, 0x0F);
    putc('s', 7, 4, 0x0F);
    putc(' ', 8, 4, 0x0F);
    putc('I', 9, 4, 0x0F);
    putc('n', 10, 4, 0x0F);
    putc('i', 11, 4, 0x0F);
    putc('t', 12, 4, 0x0F);
    putc('i', 13, 4, 0x0F);
    putc('a', 14, 4, 0x0F);
    putc('l', 15, 4, 0x0F);
    putc('i', 16, 4, 0x0F);
    putc('z', 17, 4, 0x0F);
    putc('e', 18, 4, 0x0F);
    putc('d', 19, 4, 0x0F);
    putc('!', 20, 4, 0x0F);
}