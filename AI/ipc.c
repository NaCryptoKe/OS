#include "ipc.h"
#include "task.h" /* For task_t and task management */
#include "kheap.h" /* For kmalloc */
#include "kernel.h" /* For putc (for debugging) */
#include "paging.h" /* For page_directory_entry_t */

/* Global variable for the current running task, defined in task.c */
extern task_t *current_task;
extern task_t *task_list_head;

void send_message(uint32_t dest_pid, message_t *msg) {
    /* Disable interrupts for critical section */
    asm volatile("cli");

    task_t *dest_task = task_list_head;
    while (dest_task) {
        if (dest_task->id == dest_pid) {
            break;
        }
        dest_task = dest_task->next;
    }

    if (!dest_task) {
        /* Destination task not found, handle error (for now, just re-enable interrupts and return) */
        asm volatile("sti");
        putc('E', 0, 4, 0x0F);
        putc('r', 1, 4, 0x0F);
        putc('r', 2, 4, 0x0F);
        putc('o', 3, 4, 0x0F);
        putc('r', 4, 4, 0x0F);
        putc(':', 5, 4, 0x0F);
        putc(' ', 6, 4, 0x0F);
        putc('D', 7, 4, 0x0F);
        putc('e', 8, 4, 0x0F);
        putc('s', 9, 4, 0x0F);
        putc('t', 10, 4, 0x0F);
        putc(' ', 11, 4, 0x0F);
        putc('N', 12, 4, 0x0F);
        putc('o', 13, 4, 0x0F);
        putc('t', 14, 4, 0x0F);
        putc(' ', 15, 4, 0x0F);
        putc('F', 16, 4, 0x0F);
        putc('o', 17, 4, 0x0F);
        putc('u', 18, 4, 0x0F);
        putc('n', 19, 4, 0x0F);
        putc('d', 20, 4, 0x0F);
        return;
    }

    /* Allocate space for the message and copy it */
    message_t *new_msg = (message_t*)kmalloc(sizeof(message_t));
    *new_msg = *msg;
    new_msg->source_pid = current_task->id;

    /* Add message to destination task's queue (simple linked list for now) */
    if (dest_task->message_queue == 0) {
        dest_task->message_queue = new_msg;
    } else {
        message_t *temp = dest_task->message_queue;
        while (temp->next) { /* Assuming message_t has a 'next' pointer */
            temp = temp->next;
        }
        temp->next = new_msg;
    }
    dest_task->message_count++;

    /* If destination task is blocked waiting for a message, unblock it */
    if (dest_task->state == TASK_BLOCKED) {
        dest_task->state = TASK_READY;
    }

    /* Re-enable interrupts */
    asm volatile("sti");
}

void receive_message(uint32_t *source_pid, message_t *msg) {
    /* Disable interrupts for critical section */
    asm volatile("cli");

    /* Wait until a message is available */
    while (current_task->message_count == 0) {
        current_task->state = TASK_BLOCKED;
        asm volatile("sti"); /* Re-enable interrupts while waiting */
        switch_task();       /* Yield to another task */
        asm volatile("cli"); /* Disable interrupts after returning from switch_task */
    }

    /* Get the first message from the queue */
    message_t *received_msg = current_task->message_queue;
    *msg = *received_msg; /* Copy the message data */
    *source_pid = received_msg->source_pid;

    /* Remove the message from the queue */
    current_task->message_queue = received_msg->next; /* Assuming message_t has a 'next' pointer */
    current_task->message_count--;

    kfree(received_msg); /* Free the allocated message memory */

    /* Re-enable interrupts */
    asm volatile("sti");
}