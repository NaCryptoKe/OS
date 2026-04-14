#include "gdt.h"
#include "idt.h"
#include "paging.h"
#include "task.h"
#include "syscall.h"
#include "kernel.h"
#include "ipc.h" /* For message_t */

/* Defines the VGA text buffer address */
volatile unsigned char *VGA_BUFFER = (volatile unsigned char*)0xB8000;

/* Simple function to write a character to the VGA buffer */
void putc(char c, int x, int y, unsigned char color) {
    int offset = (y * 80 + x) * 2;
    VGA_BUFFER[offset] = c;
    VGA_BUFFER[offset + 1] = color;
}

/* Clear the screen */
void clear_screen(unsigned char color) {
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        VGA_BUFFER[i] = ' ';
        VGA_BUFFER[i + 1] = color;
    }
}

/* --- User-space tasks for demonstration --- */

/* Simple server task */
void server_task() {
    putc('S', 0, 6, 0x0F); putc('E', 1, 6, 0x0F); putc('R', 2, 6, 0x0F); putc('V', 3, 6, 0x0F); putc('E', 4, 6, 0x0F); putc('R', 5, 6, 0x0F); putc(' ', 6, 6, 0x0F); putc('R', 7, 6, 0x0F); putc('U', 8, 6, 0x0F); putc('N', 9, 6, 0x0F);
    uint32_t sender_pid;
    message_t msg;
    while (1) {
        /* Receive message */
        __asm__ __volatile__ ("int $0x80" : "=a" (sender_pid) : "a" (SYS_RECEIVE), "b" (&sender_pid), "c" (&msg));

        /* Echo back the message */
        __asm__ __volatile__ ("int $0x80" : : "a" (SYS_SEND), "b" (msg.source_pid), "c" (&msg));

        putc('S', 0, 7, 0x0F); putc('V', 1, 7, 0x0F); putc('R', 2, 7, 0x0F); putc(' ', 3, 7, 0x0F); putc('R', 4, 7, 0x0F); putc('C', 5, 7, 0x0F); putc('V', 6, 7, 0x0F);
    }
}

/* Simple client task */
void client_task() {
    putc('C', 0, 8, 0x0F); putc('L', 1, 8, 0x0F); putc('I', 2, 8, 0x0F); putc('E', 3, 8, 0x0F); putc('N', 4, 8, 0x0F); putc('T', 5, 8, 0x0F); putc(' ', 6, 8, 0x0F); putc('R', 7, 8, 0x0F); putc('U', 8, 8, 0x0F); putc('N', 9, 8, 0x0F);
    message_t msg;
    uint32_t response_pid;
    uint32_t server_pid = 2; /* Assuming server is the second task created */

    while (1) {
        /* Send a message */
        msg.type = 1; /* Example message type */
        msg.payload[0] = 0xDEADBEEF;
        __asm__ __volatile__ ("int $0x80" : : "a" (SYS_SEND), "b" (server_pid), "c" (&msg));

        /* Receive response */
        __asm__ __volatile__ ("int $0x80" : "=a" (response_pid) : "a" (SYS_RECEIVE), "b" (&response_pid), "c" (&msg));

        putc('C', 0, 9, 0x0F); putc('L', 1, 9, 0x0F); putc('T', 2, 9, 0x0F); putc(' ', 3, 9, 0x0F); putc('R', 4, 9, 0x0F); putc('C', 5, 9, 0x0F); putc('V', 6, 9, 0x0F);
    }
}

/* The main entry point for the kernel */
void kernel_main() {
    /* Initialize GDT */
    init_gdt();
    /* Initialize IDT */
    init_idt();
    /* Initialize Paging */
    init_paging();
    /* Initialize Tasking */
    init_tasking();
    /* Initialize System Calls */
    init_syscalls();

    /* White text on black background */
    unsigned char color = 0x0F;

    clear_screen(color);
    putc('H', 0, 0, color);
    putc('e', 1, 0, color);
    putc('l', 2, 0, color);
    putc('l', 3, 0, color);
    putc('o', 4, 0, color);
    putc(',', 5, 0, color);
    putc(' ', 6, 0, color);
    putc('M', 7, 0, color);
    putc('i', 8, 0, color);
    putc('c', 9, 0, color);
    putc('r', 10, 0, color);
    putc('o', 11, 0, color);
    putc('K', 12, 0, color);
    putc('e', 13, 0, color);
    putc('r', 14, 0, color);
    putc('n', 15, 0, color);
    putc('e', 16, 0, color);
    putc('l', 17, 0, color);
    putc('!', 18, 0, color);

    /* Create server and client tasks */
    create_task(server_task, 0); /* PID will be 2 */
    create_task(client_task, 0); /* PID will be 3 */

    /* Loop indefinitely to keep the kernel running */
    while (1) {
        // Do nothing (scheduler will switch to other tasks)
    }
}
