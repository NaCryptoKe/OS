#ifndef IPC_H
#define IPC_H

#include <stdint.h>

/* Define a simple message structure */
typedef struct message {
    uint32_t source_pid;
    uint32_t dest_pid;
    uint32_t type; /* Message type (e.g., system call, user defined) */
    uint32_t payload[4]; /* Small payload for data */
    struct message *next; /* Pointer to the next message in the queue */
} message_t;

/* IPC function declarations */
void send_message(uint32_t dest_pid, message_t *msg);
void receive_message(uint32_t *source_pid, message_t *msg);

#endif