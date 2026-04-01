#include "common.h"
#include "vga.h"
#include "keyboard.h"

#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

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

        /* CASE 1: Handle Shift (Momentary) using Constants */
        if (scancode == SCANCODE_LSHIFT || scancode == SCANCODE_RSHIFT) {
            is_shift_pressed = true;
        }
        else if (scancode == SCANCODE_LSHIFT_RELEASE || scancode == SCANCODE_RSHIFT_RELEASE) {
            is_shift_pressed = false;
        }
        
        /* CASE 2: Handle Caps Lock (Toggle) */
        else if (scancode == SCANCODE_CAPSLOCK) {
            is_caps_locked = !is_caps_locked;
        }
        
        /* CASE 3: Handle Typing */
        else if (scancode < 0x80) {
            char c = scancode_to_ascii(scancode, is_shift_pressed ^ is_caps_locked);
            
            if (c != 0) {
                terminal_putchar(c);
            }
        }
    }
}