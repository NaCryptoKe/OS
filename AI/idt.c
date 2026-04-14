#include "idt.h"
#include "kernel.h"
#include "gdt.h"
#include "syscall.h" /* Include syscall.h for syscall_handler declaration */ /* Include gdt.h for KERNEL_CS */

/* IDT entries */
idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

/* ISRs defined in assembly */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr128(); /* Declare isr128 for system calls */

/* A generic interrupt handler for now */
void isr_handler(struct regs *r) {
    /* Print a message to indicate an interrupt occurred */
    putc('I', 0, 1, 0x0F);
    putc('R', 1, 1, 0x0F);
    putc('Q', 2, 1, 0x0F);
    putc(':', 3, 1, 0x0F);
    putc((r->int_no / 10) + '0', 4, 1, 0x0F);
    putc((r->int_no % 10) + '0', 5, 1, 0x0F);

    /* If it's a system call, call the system call handler */
    if (r->int_no == 128) {
        syscall_handler((struct syscall_regs *)r); /* Cast regs to syscall_regs */
    } else {
        /* For now, just halt the system for other interrupts */
        asm volatile("cli; hlt");
    }
}


/* Function to set up an IDT entry */
static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_low  = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].sel       = sel;
    idt_entries[num].always0   = 0;
    idt_entries[num].flags     = flags;
}

/* External assembly function to load the IDT */
extern void idt_flush(uint32_t);

void init_idt() {
    idt_ptr.limit = (sizeof(idt_entry_t) * 256) - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    /* Populate IDT with exception handlers (0-31) */
    idt_set_gate(0, (uint32_t)isr0, KERNEL_CS, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, KERNEL_CS, 0x8E);
    idt_set_gate(2, (uint32_t)isr2, KERNEL_CS, 0x8E);
    idt_set_gate(3, (uint32_t)isr3, KERNEL_CS, 0x8E);
    idt_set_gate(4, (uint32_t)isr4, KERNEL_CS, 0x8E);
    idt_set_gate(5, (uint32_t)isr5, KERNEL_CS, 0x8E);
    idt_set_gate(6, (uint32_t)isr6, KERNEL_CS, 0x8E);
    idt_set_gate(7, (uint32_t)isr7, KERNEL_CS, 0x8E);
    idt_set_gate(8, (uint32_t)isr8, KERNEL_CS, 0x8E);
    idt_set_gate(9, (uint32_t)isr9, KERNEL_CS, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, KERNEL_CS, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, KERNEL_CS, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, KERNEL_CS, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, KERNEL_CS, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, KERNEL_CS, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, KERNEL_CS, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, KERNEL_CS, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, KERNEL_CS, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, KERNEL_CS, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, KERNEL_CS, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, KERNEL_CS, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, KERNEL_CS, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, KERNEL_CS, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, KERNEL_CS, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, KERNEL_CS, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, KERNEL_CS, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, KERNEL_CS, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, KERNEL_CS, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, KERNEL_CS, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, KERNEL_CS, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, KERNEL_CS, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, KERNEL_CS, 0x8E);

    /* Set up system call interrupt (0x80) */
    idt_set_gate(128, (uint32_t)isr128, KERNEL_CS, 0x8E);

    idt_flush((uint32_t)&idt_ptr);
}
