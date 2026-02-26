#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "common.h"

uint8_t keyboard_read_scancode(void);
char scancode_to_ascii(uint8_t scancode, bool capital);

#endif