#include "common.h"
#include "vga.h"
#include "keyboard.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif


void kernel_main(void) {
    terminal_initialize();
    terminal_writestring("Kernel Booted. Try Shift AND Caps Lock!\n");

    bool is_shift_pressed = false;
    bool is_caps_locked = false;

    while (1) {
        uint8_t scancode = keyboard_read_scancode();

        /* CASE 1: Handle Shift (Momentary) */
        if (scancode == 0x2A || scancode == 0x36) {
            is_shift_pressed = true;
        }
        else if (scancode == 0xAA || scancode == 0xB6) {
            is_shift_pressed = false;
        }
        
        /* CASE 2: Handle Caps Lock (Toggle) */
        else if (scancode == 0x3A) {
            /* 0x3A is Caps Lock Press. We flip the boolean. */
            is_caps_locked = !is_caps_locked;
        }
        
        /* CASE 3: Handle Typing */
        else if (scancode < 0x80) {
            /* We pass BOTH states to the function. 
                The function will decide how to combine them.
            */
            char c = scancode_to_ascii(scancode, is_shift_pressed ^ is_caps_locked);
            
            if (c != 0) {
                terminal_putchar(c);
            }
        }
    }
}