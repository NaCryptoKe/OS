#include "kheap.h"
#include "kernel.h" /* For putc */

/* Very simple bump allocator */
extern uint32_t _end; /* Defined in linker.ld */
static uint32_t placement_address = (uint32_t)&_end;

uint32_t kmalloc_int(uint32_t size, int align, uint32_t *phys_addr) {
    if (align == 1 && (placement_address & 0xFFFFF000)) {
        /* Align the placement address to the next 4KB boundary */
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }

    if (phys_addr) {
        *phys_addr = placement_address;
    }

    uint32_t tmp = placement_address;
    placement_address += size;
    return tmp;
}

void *kmalloc(uint32_t size) {
    return (void*)kmalloc_int(size, 0, 0);
}

void kfree(void *p) {
    /* For a bump allocator, kfree is a no-op */
    (void)p;
}
