[org 0x7c00]          ; Let the assembler know we are at 0x7C00

mov si, msg           ; Point SI (Source Index) to our string's memory address
mov ah, 0x0e          ; Set BIOS teletype function once (outside the loop)

print_loop:
    lodsb             ; Load byte at [SI] into AL and increment SI
    cmp al, 0         ; Check if the character is 0 (our null-terminator)
    je done           ; If it's 0, we've reached the end of the string
    int 0x10          ; Call BIOS video interrupt to print the char in AL
    jmp print_loop    ; Repeat for the next character

done:
    jmp $             ; Infinite loop

msg: 
    db "Hello from 0x7C00!", 0 ; Our string, ending with a 0

; Padding and Magic Number
times 510 - ($-$$) db 0
dw 0xaa55