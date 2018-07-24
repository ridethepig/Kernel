#!Makefile

C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.s")
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))

CC = gcc
LD = ld
ASM = nasm

C_FLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin -fno-stack-protector -I include
LD_FLAGS = -T scripts/kernel.ld -m elf_i386 -nostdlib
ASM_FLAGS = -f elf -g -F stabs

all: $(S_OBJECTS) $(C_OBJECTS) link update_image

.c.o: 
	@echo compiling c sources $< ...
	$(CC) $(C_FLAGS) $< -o $@

.s.o: 
	@echo compiling asm sources $< ...
	$(ASM) $(ASM_FLAGS) $<

link: 
	@echo linking...
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o ikernel

.PHONY: clean
clean: 
	$(RM) $(S_OBJECTS) $(C_OBJECTS) ikernel

.PHONY: update_image
update_image:
	sudo mount floppy.img /mnt/kernel
	sudo cp ikernel /mnt/kernel/hx_kernel
	sleep 1
	sudo umount /mnt/kernel

.PHONY: mount_image
mount_image:
	sudo mount floppy.img /mnt/kernel

.PHONY: qemu
qemu: 
	qemu -fda floppy.img -boot a

.PHONY: debug
debug:
	qemu -S -s -fda floppy.img -boot a &
	sleep 1
	cgdb -x scripts/gdbinit