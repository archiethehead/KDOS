@echo off

if not exist build\ (
    mkdir build
)

nasm -f bin ./KDOS/boot.asm -o ./build/boot.bin || goto :exit
nasm -f obj ./KDOS/kernel/kernel-entry.asm -o ./build/kernel-entry.obj || goto :exit
set INCLUDE=./KDOS/kernel/include
wcl -c -0 -d0 -ms -s -wx -zl ./KDOS/kernel/utils/conio.c ./KDOS/kernel/kernel.c -fo=./build/|| goto :exit
wlink FILE ./build/kernel-entry.obj,./build/conio.obj,./build/kernel.obj FORMAT RAW BIN NAME ./build/kernel.bin OPTION NODEFAULTLIBS, START=_cstart_ || goto :exit
"Build Tool" || goto :exit

qemu-system-x86_64 -boot c -drive file=build/KDOS.img,format=raw

:exit
set /p input = PRESS ANYTHING TO CLOSE TERMINAL