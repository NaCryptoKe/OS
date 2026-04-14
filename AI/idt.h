#ifndef IDT_H
#define IDT_H

#include <stdint.h>

/* IDT entry structure */
struct idt_entry_struct {
    uint16_t base_low;    /* The lower 16 bits of the address to jump to when this interrupt fires */
    uint16_t sel;         /* Kernel segment selector */
    uint8_t  always0;     /* This must always be zero */
    uint8_t  flags;       /* Set flags (e.g., present, DPL, type) */
    uint16_t base_high;   /* The upper 16 bits of the address to jump to */
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

/* IDT pointer structure */
struct idt_ptr_struct {
    uint16_t limit;       /* The upper 16 bits of all selector limits */
    uint32_t base;        /* The address of the first idt_entry_t struct */
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

/* Function to initialize the IDT */
void init_idt();

#endif