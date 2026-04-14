.section .text

.global load_page_directory
load_page_directory:
    mov 4(%esp), %eax    /* Load the address of the page directory from the stack */
    mov %eax, %cr3       /* Load CR3 with the address of our page directory */
    ret

.global enable_paging
enable_paging:
    mov %cr0, %eax
    or $0x80000001, %eax /* Set the PG bit (bit 31) and PE bit (bit 0) */
    mov %eax, %cr0
    ret