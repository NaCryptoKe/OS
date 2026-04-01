#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "common.h"

/* Scancode Symbolic Constants */
#define SCANCODE_LSHIFT 0x2A
#define SCANCODE_RSHIFT 0x36
#define SCANCODE_LSHIFT_RELEASE 0xAA
#define SCANCODE_RSHIFT_RELEASE 0xB6
#define SCANCODE_CAPSLOCK 0x3A

uint8_t keyboard_read_scancode(void);
char scancode_to_ascii(uint8_t scancode, bool capital);

#endif