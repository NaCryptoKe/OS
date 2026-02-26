CC = i686-elf-gcc
AS = i686-elf-as

# Compilation flags
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# List of all object files to link
OBJ = boot.o kernel.o keyboard.o vga.o

all: myos.bin

# Compile the assembly bootloader
boot.o: boot.s
	$(AS) boot.s -o boot.o

# Compile each C file into its own object file
kernel.o: kernel.c
	$(CC) -c kernel.c -o kernel.o $(CFLAGS)

keyboard.o: keyboard.c
	$(CC) -c keyboard.c -o keyboard.o $(CFLAGS)

vga.o: vga.c
	$(CC) -c vga.c -o vga.o $(CFLAGS)

# Link everything together using the linker script
myos.bin: $(OBJ) linker.ld
	$(CC) -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib $(OBJ) -lgcc

clean:
	rm -f *.o myos.bin

run: myos.bin
	qemu-system-i386 -kernel myos.bin