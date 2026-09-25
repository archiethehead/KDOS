@echo off
setlocal enabledelayedexpansion

set "NORUN=false"
set "KEEPINT=false"

:args
if "%~1"=="" goto main

if /i "%~1"=="-n" (

    set "NORUN=true"

)

if /i "%~1"=="--norun" (

    set "NORUN=true"

)

if /i "%~1"=="-k" (

    set "KEEPINT=true"

)

if /i "%~1"=="--keepint" (

    set "KEEPINT=true"

)

shift
goto args


:main
if not exist build\ (
    mkdir build
)

nasm -f bin ./KDOS/boot.asm -o ./build/boot.bin || goto :exit
nasm -f obj ./KDOS/kernel/kernel-entry.asm -o ./build/kernel-entry.obj || goto :exit
set INCLUDE=./KDOS/kernel/include
wcl -q -za99 -c -0 -d0 -ms -s -wx -zl ./KDOS/kernel/utils/console-io.c ./KDOS/kernel/utils/disk.c ./KDOS/kernel/utils/memory.c ./KDOS/kernel/utils/shell.c ./KDOS/kernel/kernel.c -fo=./build/|| goto :exit
wlink OPTION QUIET FILE ./build/kernel-entry.obj,./build/disk.obj,./build/memory.obj,./build/console-io.obj,./build/shell.obj,./build/kernel.obj library 'C:\Program Files\WATCOM\lib286\dos\clibs.lib' FORMAT RAW BIN NAME ./build/kernel.bin OPTION NODEFAULTLIBS, START=_cstart_ || goto :exit
"Build Tool" || goto :exit

if "%NORUN%"=="false" (

    qemu-system-x86_64 -boot c -drive file=build/KDOS.img,format=raw

)

:exit

if "%KEEPINT%"=="false" (

    if exist *.err (
        
        del *.err

    )

    cd build

    if exist *.obj (

        del *.obj

    )

    if exist *.i (

        del *.i

    )

    if exist *.bin (

        del *.bin

    )

)

cd ..

set /p input = PRESS ANYTHING TO CLOSE TERMINAL