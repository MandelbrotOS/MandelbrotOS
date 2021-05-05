#!/usr/bin/env bash

pushd limine
    make toolchain && make
popd

cp limine/bin/limine.sys resources/limine.sys
