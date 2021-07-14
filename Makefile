ARCH = x86_64

LD = ld
CC = gcc
AS = nasm

QEMU = qemu-system-$(ARCH) -hda $(OS) -smp 4 -M q35 -soundhw pcspk -monitor stdio

OS = mandelbrotos.hdd
KERNEL = mandelbrotos.elf

ASFLAGS = -f elf64

CFLAGS := \
	-mcmodel=kernel \
	-ffreestanding \
	-Isrc/include \
	-Wall \
	-Wextra \
	-Werror \
	-lm \
	-std=gnu99 \
	-Isrc/include \
	-mgeneral-regs-only \
	-mno-red-zone \
	-pipe \
	-fno-PIC -fno-pic -no-pie \
	-fno-stack-protector \
	-Wno-implicit-fallthrough 

LDFLAGS := \
	-static \
	-Tresources/linker.ld \
	-nostdlib \
	-z max-page-size=0x1000


CFILES := $(shell find src/ -name '*.c')
ASFILES := $(shell find src/ -name '*.asm')
OFILES := $(CFILES:.c=.o) $(ASFILES:.asm=.o)

all: clean $(OS) qemu

$(OS): $(KERNEL)
	@ echo "[DD] $@"
	@ dd if=/dev/zero of=$@ bs=1M seek=64 count=0
	@ echo "[PARTED] GPT"
	@ parted -s $@ mklabel gpt
	@ echo "[PARTED] Partion"
	@ parted -s $@ mkpart primary 2048s 100%
	@ echo "[ECHFS] Format"
	@ ./resources/echfs-utils -g -p0 $@ quick-format 512
	@ echo "[ECHFS] resources/limine.cfg"
	@ ./resources/echfs-utils -g -p0 $@ import resources/limine.cfg boot/limine.cfg
	@ echo "[ECHFS] resources/limine.sys"
	@ ./resources/echfs-utils -g -p0 $@ import resources/limine.sys boot/limine.sys
	@ echo "[ECHFS] boot/"
	@ ./resources/echfs-utils -g -p0 $@ import $< boot/$<
	@ echo "[LIMINE] Install"
	@ ./resources/limine-install $@

$(KERNEL): $(OFILES) $(LIBGCC)
	@ echo "[LD] $^"
	@ $(LD) $(LDFLAGS) $^ -o $@

%.o: %.c
	@ echo "[CC] $<"
	@ $(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	@ echo "[AS] $<"
	@ $(AS) $(ASFLAGS) $< -o $@

clean:
	@ echo "[CLEAN]"
	@ rm -rf $(OFILES) $(KERNEL) $(OS)

qemu:
	@ echo "[QEMU]"
	@ $(QEMU)

