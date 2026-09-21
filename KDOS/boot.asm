[org 0x7C00]
BITS 16

KERNEL_ADDRESS equ 0x1000

cli

xor ax, ax
mov dx, ax
mov ss, ax
mov sp, 0x7C00
mov bp, sp

sti

mov ah, 0x42
mov dl, 0x80
mov si, disk_address_packet
int 0x13
jmp KERNEL_ADDRESS:0x0000

; Osdev.org. (2022).
; Disk access using the BIOS (INT 13h) - OSDev Wiki.
; [online] Available at: https://wiki.osdev.org/Disk_access_using_the_BIOS_(INT_13h)
; [Accessed 21 Sept. 2026].

disk_address_packet:
    db 0x10
    db 0x00
    dw 327
    dw 0x0000
    dw KERNEL_ADDRESS
    dw 0x0001
    dw 0x0000
    dw 0x0000
    dw 0x0000

times 510 - ($-$$) db 0
dw 0xAA55