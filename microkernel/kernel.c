#include <stdint.h>
#include <stddef.h>

static uint16_t* const VGA_BUFFER = (uint16_t*) 0xB8000;	// we're using 16 bit cause the hardware is hardwired to look at 16 bits at a time
								// The lower 8 bits are for ASCII character and the upper is for foreground and background color
static const int VGA_WIDTH = 80;
static const int VGA_HEIGHT = 25;

void kmain(void) {
	const char* str = "Welcome to the Microkernel Core!";

	for (int y = 0;	y < VGA_HEIGHT; y++) {
		for (int x = 0; x < VGA_WIDTH; x++) {
			const int index = y * VGA_WIDTH + x;
			VGA_BUFFER[index] = (uint16_t) ' ' | (uint16_t) 0x07 << 8;	// setting the background
		}
	}

	for (size_t i = 0; str[i] != '\0'; i++) {
		VGA_BUFFER[i] = (uint16_t) str[i] | (uint16_t) 0x0F << 8;		// setting the foreground and the ASCII character by using bitwise OR
	}
}
