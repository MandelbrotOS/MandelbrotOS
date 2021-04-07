KVM = 0

LD = cross/bin/x86_64-elf-ld
CC = cross/bin/x86_64-elf-gcc
AS = nasm
LIBGCC = cross/lib/gcc/x86_64-elf/9.2.0/libgcc.a

ifeq ($(KVM), 1)
	QEMU = qemu-system-x86_64 -hda $(OS) -enable-kvm
else
	QEMU = qemu-system-x86_64 -hda $(OS)
endif

OS = mandelbrotos.hdd
KERNEL = mandelbrotos.elf

ASFLAGS = -f elf64

CFLAGS := \
	-mcmodel=kernel \
	-ffreestanding \
	-Isrc/include \
	-Wall \
	-Wextra \
	-lm \

LDFLAGS := \
	-static \
	-no-pie \
	-Tresources/linker.ld \
	-nostdlib

CFILES := $(shell find src/ -name '*.c')
ASFILES := $(shell find src/ -name '*.asm')
OFILES := $(CFILES:.c=.o) $(ASFILES:.asm=.o)

ifeq ($(RUN), 1)
all: clean $(OS)
else
all: clean $(OS) qemu
endif

$(OS): $(KERNEL)
	@ echo "[DD] $@"
	@ dd if=/dev/zero of=$@ bs=1M seek=64 count=0
	@ echo "[PARTED] GPT"
	@ parted -s $@ mklabel gpt
	@ echo "[PARTED] Partion"
	@ parted -s $@ mkpart primary 2048s 100%
	@ echo "[ECHFS] Format"
	@ echfs-utils -g -p0 $@ quick-format 512
	@ echo "[ECHFS] resources/limine.cfg"
	@ echfs-utils -g -p0 $@ import resources/limine.cfg boot/limine.cfg
	@ echo "[ECHFS] resources/limine.sys"
	@ echfs-utils -g -p0 $@ import resources/limine.sys boot/limine.sys
	@ echo "[ECHFS] boot/"
	@ echfs-utils -g -p0 $@ import $< boot/$<
	@ echo "[ECHFS] foo.txt(just for testing)"
	@ echfs-utils -g -p0 $@ import resources/foo.txt tee/bar/foo.txt
	@ echo "[LIMINE] Install"
	@ limine-install $@

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
