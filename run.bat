@echo off

if not exist build\ (
    mkdir build
)

nasm -f bin ./KDOS/boot.asm -o ./build/boot.bin || goto :exit
nasm -f bin ./KDOS/kernel.asm -o ./build/kernel.bin || goto :exit
"Build Tool" || goto :exit

qemu-system-x86_64 ./build/KDOS.img

:exit
set /p input = PRESS ANYTHING TO CLOSE TERMINAL