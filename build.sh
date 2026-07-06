nasm -f bin boot.asm -o boot.bin
nasm -f bin kernel.asm -o kernel.bin
#ia16-elf-gcc -mcmodel=tiny -Os -nostdlib -Wl,--oformat=binary -o data.bin data.c

dd if=/dev/zero of=os.img bs=512 count=9000
dd if=boot.bin of=os.img conv=notrunc
dd if=kernel.bin of=os.img bs=512 seek=1 conv=notrunc

rm *.bin
qemu-system-x86_64  os.img
