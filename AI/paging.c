#include "paging.h"
#include "kernel.h" /* For putc etc. */

/* A pointer to the page directory */
page_directory_entry_t *current_page_directory = (page_directory_entry_t*)0x100000; /* 1MB boundary for alignment */

/* A simple page table (for the first 4MB of memory) */
page_table_entry_t *first_page_table = (page_table_entry_t*)0x101000; /* Just after the page directory */

/* External assembly function to load page directory and enable paging */
extern void load_page_directory(uint32_t);
extern void enable_paging();

void init_paging() {
    uint32_t i; /* loop counter - should be uint32_t */

    /* Clear the page directory */
    for (i = 0; i < 1024; i++) {
        current_page_directory[i].present = 0;
        current_page_directory[i].rw = 0;
        current_page_directory[i].user = 0;
        current_page_directory[i].write_thru = 0;
        current_page_directory[i].cache_disabled = 0;
        current_page_directory[i].accessed = 0;
        current_page_directory[i].dirty = 0;
        current_page_directory[i].page_size = 0;
        current_page_directory[i].global = 0;
        current_page_directory[i].available = 0;
        current_page_directory[i].frame = 0;
    }

    /* Identity map the first 4MB of physical memory */
    for (i = 0; i < 1024; i++) {
        first_page_table[i].present = 1;   /* The page is present */
        first_page_table[i].rw = 1;        /* Read/write enabled */
        first_page_table[i].user = 0;      /* Supervisor only */
        first_page_table[i].frame = i;     /* Frame address is identity mapped */
        /* ... other flags can be set to 0 initially */
    }

    /* Map the first page table into the page directory */
    current_page_directory[0].present = 1;
    current_page_directory[0].rw = 1;
    current_page_directory[0].user = 0;
    current_page_directory[0].frame = ((uint32_t)first_page_table) >> 12;

    /* Load the page directory */
    load_page_directory((uint32_t)current_page_directory);

    /* Enable paging */
    enable_paging();

    putc('P', 0, 2, 0x0F);
    putc('a', 1, 2, 0x0F);
    putc('g', 2, 2, 0x0F);
    putc('i', 3, 2, 0x0F);
    putc('n', 4, 2, 0x0F);
    putc('g', 5, 2, 0x0F);
    putc(' ', 6, 2, 0x0F);
    putc('E', 7, 2, 0x0F);
    putc('n', 8, 2, 0x0F);
    putc('a', 9, 2, 0x0F);
    putc('b', 10, 2, 0x0F);
    putc('l', 11, 2, 0x0F);
    putc('e', 12, 2, 0x0F);
    putc('d', 13, 2, 0x0F);
    putc('!', 14, 2, 0x0F);
}
