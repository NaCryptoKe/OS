#include <stdint.h>

struct gdt_entry_struct {
	uint16_t limit_low;		// The lower 16 bits of the limit.
	uint16_t base_low;		// The lower 16 bits of the base.
	uint8_t base_middle;		// The next 8 bits of the base.
	uint8_t access;			// Access flags (is it a ring 0 or ring 3?)
	uint8_t granularity; 
	uint8_t base_high;		// The last 8 bits of the base.
} __attribute__((packed));

// A pointer to the GDT table for the CPU
struct gdt_ptr_struct {
    uint16_t limit;               // The size of the table
    uint32_t base;                // The address of the first entry
} __attribute__((packed));

struct gdt_entry_struct gdt_entries[5];
struct gdt_ptr_struct   gdt_ptr;

// Function to set up a single GDT gate
void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;
}

void init_gdt() {
    gdt_ptr.limit = (sizeof(struct gdt_entry_struct) * 5) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;

    // 1. Null segment (Required by CPU)
    gdt_set_gate(0, 0, 0, 0, 0);                
    // 2. Code segment (Kernel mode)
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); 
    // 3. Data segment (Kernel mode)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); 
    // 4. User mode Code segment (Used later for apps)
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); 
    // 5. User mode Data segment (Used later for apps)
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); 

    // We need an assembly function to actually tell the CPU to use this
    gdt_flush((uint32_t)&gdt_ptr);
}
