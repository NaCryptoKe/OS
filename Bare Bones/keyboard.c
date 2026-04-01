#include "keyboard.h"

/* Standard US QWERTY mapping for XT scancodes */
static const char scancode_map_lower[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', /* 0x00 - 0x0E */
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',   /* 0x0F - 0x1C */
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  0,      /* 0x1D - 0x2A */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0, '*', 0,      /* 0x2B - 0x38 */
    ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                       /* 0x39 - ... */
};

static const char scancode_map_upper[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', 
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',   
    0,  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',  0,      
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',   0, '*', 0,      
    ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                      
};

/* Check if a key has been pressed */
uint8_t keyboard_read_scancode(void) {
    // Bit 0 of port 0x64 tells us if the output buffer is full (data is ready)
    while (!(inb(0x64) & 1)); 
    return inb(0x60);
}

/* O(1) Translation table for demonstration */
char scancode_to_ascii(uint8_t scancode, bool capital) {
    if (scancode >= 128) {
        return 0; 
    }
    return capital ? scancode_map_upper[scancode] : scancode_map_lower[scancode];
}