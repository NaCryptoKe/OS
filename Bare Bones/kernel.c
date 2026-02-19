#include <stdbool.h>
/*
    stdbool.h (Standard Boolean)

    By default, original C didn't have a real true or false type (it just used 0 and 1).

    This header defines:

        bool: The data type.
        true: Defined as 1.
        false: Defined as 0.

    It makes your kernel code much more readable. Instead of writing if (is_running == 1), you can write if (is_running).
*/

#include <stddef.h>
/*
    stddef.h (Standard Definitions)

    This header provides a few "quality of life" definitions that are used everywhere in C. The two biggest ones are:

        NULL: Defined as (void*)0. It represents a pointer that points to nothing.
        size_t: This is an unsigned integer type used to represent the size of objects in memory. On a 32-bit system, it’s a 32-bit number; on a 64-bit system, it’s 64-bit. It’s the "proper" way to handle memory sizes or array indices.
*/

#include <stdint.h>
/*
    stdint.h (Standard Integers)

    This is the most important one for a kernel developer.

    In normal C, an int might be 2 bytes on one computer and 4 bytes on another. When you are talking to hardware, you need to be exact. If a hardware register is exactly 32 bits wide, you can't guess.

    stdint.h gives you "fixed-width" types:

        uint8_t: An unsigned 8-bit integer (exactly 1 byte). Great for characters or raw bytes.
        uint16_t: Exactly 16 bits.
        uint32_t: Exactly 32 bits.
        uint64_t: Exactly 64 bits.

    Why use this? If you are writing to the VGA screen buffer (which uses 16-bit values for colors and characters), you use uint16_t so you know for a fact you aren't accidentally writing too much or too little data.
*/

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

/*
    * static: This means the function is only visible to the file it's in.
    
*/
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000 

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) 
{
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}

void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}

void kernel_main(void) 
{
	/* Initialize terminal interface */
	terminal_initialize();

	/* Newline support is left as an exercise. */
	terminal_writestring("Hello, kernel World!\n");
}
