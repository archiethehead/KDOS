[org 0x7c00]
BITS 16

cli

xor ax, ax
mov ss, ax
mov sp, 0x7C00
mov bp, sp

sti

jmp $

times 510 - ($-$$) db 0
dw 0xaa55