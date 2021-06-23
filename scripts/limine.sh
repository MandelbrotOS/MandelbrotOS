#!/usr/bin/env bash
cd ../limine
make toolchain
make
mv bin/limine-install ../resources
mv bin/limine.sys ../resources
mv bin/BOOTX64.EFI ../resources
cd ..
