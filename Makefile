GCC_PREFIX = x86_64-elf
ARCH = x86_64

LD = $(GCC_PREFIX)-ld
CC = $(GCC_PREFIX)-gcc
AS = nasm

QEMU = qemu-system-$(ARCH) -cdrom $(ISO) -smp 2 -M q35 -soundhw pcspk -monitor stdio -enable-kvm

ISO = mandelbrot.iso
KERNEL = $(BUILD_DIRECTORY)/mandelbrotos.elf

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
        -fno-pic -no-pie \
	-fno-stack-protector \
	-Wno-implicit-fallthrough 

LDFLAGS := \
	-static \
	-Tresources/linker.ld \
	-nostdlib \
	-z max-page-size=0x1000


CFILES := $(shell find src/ -name '*.c')
ASFILES := $(shell find src/ -name '*.asm')

OBJ = $(patsubst %.c, $(BUILD_DIRECTORY)/%.c.o, $(CFILES)) \
        $(patsubst %.asm, $(BUILD_DIRECTORY)/%.asm.o, $(ASFILES))

BUILD_DIRECTORY := build
DIRECTORY_GUARD = @mkdir -p $(@D)

all: $(ISO)

$(ISO): $(KERNEL)
	scripts/make-image.sh > /dev/null 2>&1

$(KERNEL): $(OBJ) $(LIBGCC)
	$(LD) $(LDFLAGS) $^ -o $@

$(BUILD_DIRECTORY)/%.c.o: %.c
	$(DIRECTORY_GUARD)
	$(CC) $(CFLAGS) -c $< -o $@


$(BUILD_DIRECTORY)/%.asm.o: %.asm
	$(DIRECTORY_GUARD)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIRECTORY) $(ISO)

run: $(ISO)
	$(QEMU)

