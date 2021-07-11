#!/usr/bin/env bash

set -e
set -x

cores=$(($(nproc) + 1))
load=$(nproc)

TARGET=x86_64-elf
BINUTILSVERSION=2.36.1
GCCVERSION=11.1.0
PREFIX="$(pwd)/cross"

if [ -z "$MAKEFLAGS" ]; then
	MAKEFLAGS="$1"
fi
export MAKEFLAGS

export PATH="$PREFIX/bin:$PATH"

if [ -x "$(command -v gmake)" ]; then
    mkdir -p "$PREFIX/bin"
    cat <<EOF >"$PREFIX/bin/make"
#!/usr/bin/env sh
gmake "\$@"
EOF
    chmod +x "$PREFIX/bin/make"
fi

mkdir -p buildcc
cd buildcc

if [ ! -f binutils-$BINUTILSVERSION.tar.gz ]; then
    wget https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILSVERSION.tar.gz
fi
if [ ! -f gcc-$GCCVERSION.tar.gz ]; then
    wget https://ftp.gnu.org/gnu/gcc/gcc-$GCCVERSION/gcc-$GCCVERSION.tar.gz
fi

tar -xf binutils-$BINUTILSVERSION.tar.gz
tar -xf gcc-$GCCVERSION.tar.gz

rm -rf buildcc-gcc build-binutils

mkdir buildcc-binutils
cd buildcc-binutils
../binutils-$BINUTILSVERSION/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j$cores -l$load
make install -j$cores -l$load
cd ..

cd gcc-$GCCVERSION
contrib/download_prerequisites
cd ..
mkdir buildcc-gcc
cd buildcc-gcc
../gcc-$GCCVERSION/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c --without-headers
make all-gcc -j$cores -l$load
make all-target-libgcc -j$cores -l$load
make install-gcc -j$cores -l$load
make install-target-libgcc -j$cores -l$load
cd ..

rm -rf buildcc

