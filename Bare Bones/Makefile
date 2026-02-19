CC = i686-elf-gcc
AS = i686-elf-as

all: myos.bin

boot.o: boot.s
	$(AS) boot.s -o boot.o

kernel.o: kernel.c
	$(CC) -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

myos.bin: boot.o kernel.o linker.ld
	$(CC) -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

clean:
	rm -f *.o myos.bin

run: myos.bin
	qemu-system-i386 -kernel myos.bin