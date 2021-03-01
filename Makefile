ARCH=i386

CROSS=cross

CC=$(CROSS)/bin/$(ARCH)-elf-gcc
LD=$(CROSS)/bin/$(ARCH)-elf-ld

AS=nasm
GRUB_MKRESCUE?=grub-mkrescue

GCC_VERSION=10.2.0

LIBGCC=$(CROSS)/lib/gcc/$(ARCH)-elf/$(GCC_VERSION)/libgcc.a

CWARNINGS= -Wno-unused-variable -Wno-attributes -Wno-unused-parameter -Wno-pointer-to-int-cast -Wno-int-conversion
CFLAGS=-m32 -std=c99 -fcommon -nostdlib -nostartfiles -nodefaultlibs -ffreestanding -fno-builtin -fno-omit-frame-pointer $(CWARNINGS) $(CINCLUDES) -Isrc/include/
DFLAGS=-g -DDEBUG -O0

ASFLAGS=-f elf

LDFLAGS=-melf_i386 -nostdlib -T src/arch/$(ARCH)/linker.ld -Map=build/mandelbrotos.map 

CSOURCES:=\
$(wildcard src/kernel/*.c)\
src/string/string.c\
$(wildcard src/arch/$(ARCH)/*.c)

ASOURCES:=\
src/arch/$(ARCH)/boot.S\

NASMSOURCES:=\
src/arch/$(ARCH)/interrupts.asm\

COBJECTS:=$(CSOURCES:%.c=%.o)
AOBJECTS:=$(ASOURCES:%.S=%.ao)
NASMOBJECTS:=$(NASMSOURCES:%.asm=%.aso)

OBJS=\
$(ARCH_OBJS)\
$(KERNEL_OBJS)\

KERNEL=build/mandelbrotos.elf

STAGE2=build/stage2_eltorito
GENISOIMAGE=genisoimage

ISO=build/mandelbrotos.iso
QEMU=qemu-system-x86_64 -cdrom $(ISO)

.PHONY: all build clean qemu

all: $(ISO)

$(ISO): $(KERNEL)
	mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/$(notdir $(KERNEL))
	cp resources/grub.cfg iso/boot/grub
	$(GRUB_MKRESCUE) -o $(ISO) iso

$(KERNEL): $(COBJECTS) $(AOBJECTS) $(NASMOBJECTS)
	mkdir -p build
	$(LD) $(LDFLAGS) $(AOBJECTS) $(COBJECTS) $(NASMOBJECTS) $(LIBGCC) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.ao: %.S
	$(CC) $(CFLAGS) -o $@ -c $<

%.aso: %.asm
	$(AS) $(ASFLAGS) -o $@ $<

qemu: $(ISO) 
	$(QEMU)

clean:
	rm -rf iso
	rm -rf build
	find src/ -type f -name "*o" -delete
	rm -f $(ISO)
