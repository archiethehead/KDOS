@echo off

mkdir build
nasm -f bin ./KDOS/boot.asm -o ./build/boot.bin
nasm -f bin ./KDOS/kernel.asm -o ./build/kernel.bin
"Build Tool"

qemu-system-x86_64 ./build/KDOS.img