#!/usr/bin/env bash
./cross-compiler.sh
./limine-and-limine-toolchain.sh
./echfs.sh

sed -i 's/CC = gcc/CC = cross/bin/$(ARCH)-elf-gcc'
sed -i 's/LD = ld/LD = cross/bin/$(ARCH)-elf-ld'
