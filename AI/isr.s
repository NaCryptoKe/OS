.section .text

.global idt_flush
idt_flush:
    mov 4(%esp), %eax /* Load the address of the IDT pointer from the stack */
    lidt (%eax)      /* Load the IDT */
    ret

/* Common stub for all ISRs */
.macro ISR_COMMON_STUB
    pusha                  /* Push all general purpose registers */
    push %ds
    push %es
    push %fs
    push %gs

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    call isr_handler       /* Call the C interrupt handler */

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa                   /* Pop all general purpose registers */
    add $8, %esp           /* Pop error code and interrupt number (if present) */
    iret                   /* Return from interrupt */
.endm

/* Define ISRs for exceptions 0-31 */

.global isr0
isr0:
    cli
    push $0       /* Push a dummy error code if not present */
    push $0       /* Push interrupt number */
    ISR_COMMON_STUB

.global isr1
isr1:
    cli
    push $0
    push $1
    ISR_COMMON_STUB

.global isr2
isr2:
    cli
    push $0
    push $2
    ISR_COMMON_STUB

.global isr3
isr3:
    cli
    push $0
    push $3
    ISR_COMMON_STUB

.global isr4
isr4:
    cli
    push $0
    push $4
    ISR_COMMON_STUB

.global isr5
isr5:
    cli
    push $0
    push $5
    ISR_COMMON_STUB

.global isr6
isr6:
    cli
    push $0
    push $6
    ISR_COMMON_STUB

.global isr7
isr7:
    cli
    push $0
    push $7
    ISR_COMMON_STUB

.global isr8
isr8:
    cli
    push $8
    ISR_COMMON_STUB

.global isr9
isr9:
    cli
    push $0
    push $9
    ISR_COMMON_STUB

.global isr10
isr10:
    cli
    push $10
    ISR_COMMON_STUB

.global isr11
isr11:
    cli
    push $11
    ISR_COMMON_STUB

.global isr12
isr12:
    cli
    push $12
    ISR_COMMON_STUB

.global isr13
isr13:
    cli
    push $13
    ISR_COMMON_STUB

.global isr14
isr14:
    cli
    push $14
    ISR_COMMON_STUB

.global isr15
isr15:
    cli
    push $0
    push $15
    ISR_COMMON_STUB

.global isr16
isr16:
    cli
    push $0
    push $16
    ISR_COMMON_STUB

.global isr17
isr17:
    cli
    push $0
    push $17
    ISR_COMMON_STUB

.global isr18
isr18:
    cli
    push $0
    push $18
    ISR_COMMON_STUB

.global isr19
isr19:
    cli
    push $0
    push $19
    ISR_COMMON_STUB

.global isr20
isr20:
    cli
    push $0
    push $20
    ISR_COMMON_STUB

.global isr21
isr21:
    cli
    push $0
    push $21
    ISR_COMMON_STUB

.global isr22
isr22:
    cli
    push $0
    push $22
    ISR_COMMON_STUB

.global isr23
isr23:
    cli
    push $0
    push $23
    ISR_COMMON_STUB

.global isr24
isr24:
    cli
    push $0
    push $24
    ISR_COMMON_STUB

.global isr25
isr25:
    cli
    push $0
    push $25
    ISR_COMMON_STUB

.global isr26
isr26:
    cli
    push $0
    push $26
    ISR_COMMON_STUB

.global isr27
isr27:
    cli
    push $0
    push $27
    ISR_COMMON_STUB

.global isr28
isr28:
    cli
    push $0
    push $28
    ISR_COMMON_STUB

.global isr29
isr29:
    cli
    push $0
    push $29
    ISR_COMMON_STUB

.global isr30
isr30:
    cli
    push $0
    push $30
    ISR_COMMON_STUB

.global isr31
isr31:
    cli
    push $0
    push $31
    ISR_COMMON_STUB

.global isr128 /* For System Calls (INT 0x80) */
isr128:
    cli
    push $0
    push $128
    ISR_COMMON_STUB
