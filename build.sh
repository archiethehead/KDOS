nasm -f bin boot.asm -o boot.bin
nasm -f bin kernel.asm -o kernel.bin
#ia16-elf-gcc -mcmodel=tiny -Os -nostdlib -Wl,--oformat=binary -o data.bin data.c


dd if=/dev/zero of=os.flp bs=512 count=2880
mkfs.fat -F 12 os.flp
dd if=boot.bin of=os.flp conv=notrunc
mcopy -i os.flp kernel.bin ::/

#dd if=kernel.bin of=os.flp bs=512 seek=1 conv=notrunc

rm *.bin
qemu-system-x86_64 -boot a -fda os.flp
