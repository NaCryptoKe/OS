#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include "ipc.h" /* Include ipc.h for message_t */
#include "paging.h" /* Include paging.h for page_directory_entry_t */

/* Defines the context of a task */
struct task_context {
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;
    /* Other registers will be pushed/popped by the context switch assembly */
};
typedef struct task_context task_context_t;

/* Defines the state of a task */
enum task_state {
    TASK_RUNNING,
    TASK_READY,
    TASK_BLOCKED,
    TASK_TERMINATED
};

/* Defines the structure for a task (process/thread) */
struct task_struct {
    uint32_t id;                /* Unique task ID */
    task_context_t context;     /* CPU context for the task */
    enum task_state state;      /* Current state of the task */
    page_directory_entry_t *page_directory;   /* Pointer to the task's page directory */
    struct task_struct *next;   /* Pointer to the next task in the scheduler list */
    message_t *message_queue;   /* Pointer to a queue of messages for this task */
    uint32_t message_count;     /* Number of messages in the queue */
};
typedef struct task_struct task_t;

/* Function to initialize the tasking system */
void init_tasking();

/* Function to create a new task */
task_t* create_task(void (*entry_point)(), uint32_t flags);

/* Function to switch to the next task */
void switch_task();

#endif