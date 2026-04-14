#include "task.h"
#include "gdt.h" /* For KERNEL_CS and KERNEL_DS */
#include "paging.h" /* For setting up page directories and current_page_directory */
#include "kernel.h" /* For putc */
#include "kheap.h" /* For kmalloc */
#include "ipc.h"

/* The currently running task */
task_t *current_task;

/* The head of the task linked list */
task_t *task_list_head;

/* The next available process ID */
uint32_t next_pid = 1;

extern uint32_t read_eip();
extern void _switch_context(task_context_t *old_context, task_context_t *new_context);
extern void load_page_directory(uint32_t); /* Explicit declaration */

void init_tasking() {
    /* Disable interrupts */
    asm volatile("cli");

    /* Create the first task (the kernel itself) */
    current_task = (task_t*)kmalloc(sizeof(task_t));
    current_task->id = next_pid++;
    current_task->state = TASK_RUNNING;
    current_task->page_directory = current_page_directory; /* Needs to be global in paging.c */
    current_task->message_queue = 0;
    current_task->message_count = 0;
    current_task->next = 0; /* No other tasks yet */

    task_list_head = current_task;

    /* Enable interrupts */
    asm volatile("sti");

    putc('T', 0, 3, 0x0F);
    putc('a', 1, 3, 0x0F);
    putc('s', 2, 3, 0x0F);
    putc('k', 3, 3, 0x0F);
    putc('i', 4, 3, 0x0F);
    putc('n', 5, 3, 0x0F);
    putc('g', 6, 3, 0x0F);
    putc(' ', 7, 3, 0x0F);
    putc('I', 8, 3, 0x0F);
    putc('n', 9, 3, 0x0F);
    putc('i', 10, 3, 0x0F);
    putc('t', 11, 3, 0x0F);
    putc('i', 12, 3, 0x0F);
    putc('a', 13, 3, 0x0F);
    putc('l', 14, 3, 0x0F);
    putc('i', 15, 3, 0x0F);
    putc('z', 16, 3, 0x0F);
    putc('e', 17, 3, 0x0F);
    putc('d', 18, 3, 0x0F);
    putc('!', 19, 3, 0x0F);
}

task_t* create_task(void (*entry_point)(), uint32_t flags) {
    /* Disable interrupts */
    asm volatile("cli");

    task_t *new_task = (task_t*)kmalloc(sizeof(task_t));
    new_task->id = next_pid++;
    new_task->state = TASK_READY;

    /* Setup kernel stack for the new task */
    uint32_t stack = (uint32_t)kmalloc(4096) + 4096; /* 4KB stack, grow downwards */

    /* Copy kernel's page directory for now (will implement proper copy-on-write later) */
    new_task->page_directory = current_page_directory;
    new_task->message_queue = 0;
    new_task->message_count = 0;

    /* Setup initial context */
    new_task->context.eip = (uint32_t)entry_point;
    new_task->context.esp = stack;
    new_task->context.ebp = stack;

    /* Add to task list */
    task_t *temp = task_list_head;
    while (temp->next) {
        temp = temp->next;
    }
    temp->next = new_task;
    new_task->next = 0;

    /* Enable interrupts */
    asm volatile("sti");

    return new_task;
}

void switch_task() {
    /* If no current task, or interrupts are off, or next task is null, do nothing */
    if (!current_task || current_task->state == TASK_TERMINATED || !task_list_head->next) {
        return;
    }

    /* Save current task's EIP (this will be the return address from _switch_context) */
    if (current_task->context.eip == 0) {
        current_task->context.eip = read_eip();
    }

    /* Find the next ready task */
    task_t *prev_task = current_task;
    task_t *next_task = current_task->next;
    while (next_task && next_task->state != TASK_READY) {
        next_task = next_task->next;
    }

    /* If no ready task found, go back to the head of the list (kernel task is always ready) */
    if (!next_task) {
        next_task = task_list_head;
        while (next_task && next_task->state != TASK_READY) {
            next_task = next_task->next;
        }
    }

    /* If still no ready task (shouldn't happen if kernel task is always ready), return */
    if (!next_task) {
        return;
    }

    current_task = next_task;

    /* Switch to new task's page directory */
    load_page_directory((uint32_t)current_task->page_directory);

    /* Perform context switch */
    _switch_context(&prev_task->context, &current_task->context);
}