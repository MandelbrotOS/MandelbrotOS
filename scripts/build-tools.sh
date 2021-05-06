#!/bin/bash

exitfail() {
    EXIT_CODE=$?
    if [[ $EXIT_CODE != 0 ]]; then
        echo -e "\nPrevious command failed with exit code $EXIT_CODE; quitting..."
        exit 1
    fi
}

./scripts/build-cross-compiler.sh
exitfail

./scripts/build-echfs.sh
exitfail

./scripts/build-limine-install.sh
exitfail

rm -rf cross/build
