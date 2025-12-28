#!/bin/sh

if [ $# != 1 ]; then
    echo "Usage: $0 <preset>" >&2
    exit 1
fi

file="$(dirname "$0")/build/$1/program.elf"
if [ ! -e "$file" ]; then
    echo "$file: does not exist" >&2
    exit 1
fi
qemu-system-riscv64 -machine virt -nographic -bios "$file"
