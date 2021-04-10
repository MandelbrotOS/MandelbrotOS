#!/usr/bin/env bash

git clone https://github.com/MandelbrotOS/limine.git --branch=master
cd limine && make toolchain && make && sudo make install && cd ..
cp limine/bin/limine.sys ../resources/limine.sys
