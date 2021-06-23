#!/usr/bin/env bash
cd ../echfs
make echfs-utils 
make mkfs.echfs
mv echfs-utils ../resources
