@echo off

if not exist build\ (
    mkdir build
)

nasm -f bin ./KDOS/boot.asm -o ./build/boot.bin || goto :exit
nasm -f obj ./KDOS/kernel/kernel-entry.asm -o ./build/kernel-entry.obj || goto :exit
set INCLUDE=./KDOS/kernel/include
wcl -za99 -c -0 -d0 -ms -s -wx -zl ./KDOS/kernel/utils/disk.c ./KDOS/kernel/utils/memory.c ./KDOS/kernel/utils/shell.c ./KDOS/kernel/utils/console-io.c ./KDOS/kernel/kernel.c -fo=./build/|| goto :exit
wlink FILE ./build/kernel-entry.obj,./build/disk.obj,./build/memory.obj,./build/console-io.obj,./build/shell.obj,./build/kernel.obj library 'C:\Program Files\WATCOM\lib286\dos\clibs.lib' FORMAT RAW BIN NAME ./build/kernel.bin OPTION NODEFAULTLIBS, START=_cstart_ || goto :exit
"Build Tool" || goto :exit

qemu-system-x86_64 -boot c -drive file=build/KDOS.img,format=raw


:exit

del *.err
cd build
del *.obj
cd ..

set /p input = PRESS ANYTHING TO CLOSE TERMINAL