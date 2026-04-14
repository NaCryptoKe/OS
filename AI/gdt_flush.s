.section .text
.global gdt_flush
gdt_flush:
    mov 4(%esp), %eax  /* Load the address of the GDT pointer from the stack */
    lgdt (%eax)        /* Load the GDT */

    /* Reload segment registers */
    mov $0x10, %ax     /* 0x10 is the offset of the kernel data segment */
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ljmp $0x08, $.reload_cs /* 0x08 is the offset of the kernel code segment */

.reload_cs:
    ret