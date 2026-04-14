.section .text
.global read_eip
read_eip:
    mov (%esp), %eax /* EIP is the return address on the stack */
    ret
