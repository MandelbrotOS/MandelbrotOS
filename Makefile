ARCH = x86_64

BUILDDIR = ./build
BINDIR   = ./bin

KERNEL = ./bin/mandelbrotos.elf
OS = ./bin/mandelbrotos.hdd

CC = gcc
LD = ld
AS = nasm

QEMU = qemu-system-$(ARCH) -smp 2 -M q35 -soundhw pcspk -monitor stdio

CFLAGS := \
	-Wall \
	-Wextra \
	-Werror \
	-Wno-implicit-fallthrough \
	-std=gnu99 \
	-Isrc/include \
	-ffreestanding \
	-fno-pic -no-pie \
	-fno-stack-protector \
	-mcmodel=kernel \
	-mgeneral-regs-only \
	-mno-red-zone \
	-pipe \
	-lm

LDFLAGS := \
	-static \
	-Tresources/linker.ld \
	-nostdlib \
	-z max-page-size=0x1000

ASFLAGS = -f elf64

CFILES  := $(shell find src/ -type f -name '*.c')
ASFILES := $(shell find src/ -type f -name '*.asm')
OBJS    := $(CFILES:%.c=./build/%.o) $(ASFILES:%.asm=./build/%.o)

.DEFAULT: all
all: submodules $(OS) qemu
	
$(OS): $(KERNEL)
	@ echo "[DD] $@"
	@ dd if=/dev/zero of=$@ bs=1M seek=64 count=0
	@ echo "[PARTED] GPT"
	@ parted -s $@ mklabel gpt
	@ echo "[PARTED] Partion"
	@ parted -s $@ mkpart primary 2048s 100%
	@ echo "[ECHFS] Format"
	@ ./echfs/echfs-utils -g -p0 $@ quick-format 512
	@ echo "[ECHFS] resources/limine.cfg"
	@ ./echfs/echfs-utils -g -p0 $@ import resources/limine.cfg boot/limine.cfg
	@ echo "[ECHFS] resources/limine.sys"
	@ ./echfs/echfs-utils -g -p0 $@ import resources/limine.sys boot/limine.sys
	@ echo "[ECHFS] boot/"
	@ ./echfs/echfs-utils -g -p0 $@ import $< boot/mandelbrotos.elf
	@ echo "[LIMINE] Install"
	@ ./limine/limine-install $@

$(KERNEL): $(OBJS) $(LIBGCC)
	@ mkdir -p $(@D)
	@ echo "[LD] $^"
	@ $(LD) $(LDFLAGS) $^ -o $@

./build/%.o: %.c
	@ mkdir -p $(@D)
	@ echo "[CC] $<"
	$(CC) $(CFLAGS) -c $< -o $@

./build/%.o: %.asm
	@ mkdir -p $(@D)
	@ echo "[AS] $<"
	@ $(AS) $(ASFLAGS) $< -o $@

.PHONY: submodules
submodules:
	@ git submodule update --init --depth 1
	@ git submodule update --remote --depth 1
	@ make -C echfs echfs-utils

.PHONY: clean
clean:
	@ echo "[CLEAN]"
	@ rm -rf $(BUILDDIR) $(BINDIR) ./image ./tools
	@ make -C echfs clean

.PHONY: qemu
qemu:
	@ echo "[QEMU]"
	@ $(QEMU) -hda $(OS)

./tools/OVMF.fd:
	@ mkdir -p ./tools
	@ wget -P ./tools https://efi.akeo.ie/OVMF/OVMF-X64.zip
	@ cd ./tools && unzip OVMF-X64.zip
	@ rm -rf OVMF-X64.zip readme.txt

.PHONY: test
test: $(KERNEL) ./tools/OVMF.fd
	@ mkdir -p ./image/EFI/BOOT
	@ mkdir -p ./image/boot
	@ cp ./limine/BOOTX64.EFI ./image/EFI/BOOT
	@ cp ./resources/limine.cfg ./image
	@ cp $< ./image/boot/mandelbrotos.elf
	@ $(QEMU) -hda fat:rw:image -bios tools/OVMF.fd

