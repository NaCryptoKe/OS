.section .text

.global _switch_context
_switch_context:
    /* Save current task's context */
    mov 4(%esp), %eax      /* Get old_task_context pointer */
    mov %esp, (%eax)       /* Save ESP */
    mov %ebp, 4(%eax)      /* Save EBP */
    mov (%esp), %ebx       /* Save EIP (return address from call) */
    mov %ebx, 8(%eax)

    /* Load new task's context */
    mov 8(%esp), %eax      /* Get new_task_context pointer */
    mov (%eax), %esp       /* Load ESP */
    mov 4(%eax), %ebp      /* Load EBP */
    mov 8(%eax), %ecx      /* Load EIP into ECX */
    jmp *%ecx              /* Jump to new EIP */
