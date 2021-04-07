#!/bin/bash

./build-cross-compiler.sh
./build-echfs.sh
./build-limine-install.sh
rm -rf build echfs limine