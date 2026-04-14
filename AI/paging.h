#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

/* Page directory entry structure */
typedef struct page_directory_entry {
    uint32_t present    : 1;   /* 0 = Not present in memory, 1 = Present in memory */
    uint32_t rw         : 1;   /* 0 = Read-only, 1 = Read/Write */
    uint32_t user       : 1;   /* 0 = Supervisor, 1 = User */
    uint32_t write_thru : 1;   /* 0 = Write-back caching, 1 = Write-through caching */
    uint32_t cache_disabled : 1; /* 0 = Caching enabled, 1 = Caching disabled */
    uint32_t accessed   : 1;   /* 0 = Not accessed, 1 = Accessed (set by CPU) */
    uint32_t dirty      : 1;   /* 0 = Not dirty, 1 = Dirty (set by CPU) */
    uint32_t page_size  : 1;   /* 0 = 4KB page, 1 = 4MB page */
    uint32_t global     : 1;   /* 0 = Not global, 1 = Global (for TLB caching) */
    uint32_t available  : 3;   /* Ignored (available for OS use) */
    uint32_t frame      : 20;  /* Frame address (physical page address) */
} page_directory_entry_t;

/* Page table entry structure */
typedef struct page_table_entry {
    uint32_t present    : 1;
    uint32_t rw         : 1;
    uint32_t user       : 1;
    uint32_t write_thru : 1;
    uint32_t cache_disabled : 1;
    uint32_t accessed   : 1;
    uint32_t dirty      : 1;
    uint32_t pat        : 1;   /* Page Attribute Table (Pentium Pro and above) */
    uint32_t global     : 1;
    uint32_t available  : 3;
    uint32_t frame      : 20;
} page_table_entry_t;

extern page_directory_entry_t *current_page_directory;

/* Function to initialize paging */
void init_paging();

#endif