ARCH = i386
TARGET = $(ARCH)-elf
CC = $(TARGET)-gcc
AS = $(TARGET)-as
LD = $(TARGET)-ld

QEMU = qemu-system-$(ARCH)

KERNEL_C_SRCS = kernel.c gdt.c idt.c paging.c kheap.c ipc.c task.c syscall.c
KERNEL_ASM_SRCS = boot.s gdt_flush.s isr.s paging_asm.s task_switch.s read_eip.s

OBJS = boot.o gdt_flush.o isr.o paging_asm.o task_switch.o read_eip.o kernel.o gdt.o idt.o paging.o kheap.o ipc.o task.o syscall.o

KERNEL = kernel.bin

.PHONY: all clean run

all: $(KERNEL)

$(KERNEL): $(OBJS) linker.ld
	$(LD) -T linker.ld -o $@ $(OBJS)

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra

%.o: %.s
	$(AS) $< -o $@

run: $(KERNEL)
	$(QEMU) -kernel $(KERNEL)

clean:
	rm -f $(OBJS) $(KERNEL)
