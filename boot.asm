[org 0x7c00]
BITS 16

jmp short boot
nop

;	BIOS Parameter Block, do NOT erase.
;	Credit --> https://medium.com/@atgeorgedeming/a-boot-with-fat12-file-system-9072fb43c4a3
;
;	|
;	|
;	V

BS_OEMName db "mkfs.fat"
BPB_BytesPerSec dw 512
BPB_SecPerClus db 1
BPB_RsvdSecCnt dw 1
PB_NumFATs db 2
BPB_RootEntCnt dw 224
BPB_TotSec16 dw 2880
BPB_Media db 0xf0
BPB_FATSz16 dw 9
BPB_SecPerTrk dw 18
BPB_NumHeads dw 2
BPB_HiddSec dd 0
BPB_TotSec32 dd 0
BS_DrvNum db 0
BS_Reserved1 db 0
BS_BootSig db 0x29
BS_VolID dd 0
BS_VolLab db "boot loader"
BS_FileSysType db "FAT12"

boot:

	xor ax, ax
	mov ds, ax
	mov es, ax

	mov ax, 0x0000
	mov es, ax
	mov bx, kernel_offset
	
	mov si, mbr ; <-- Just to tell us the MBR is being ran at all. 
	call strout	 ;     Death to writing files into floppy mounts.

	mov dl, 0x00 ;drivenum
	mov ch, 0x00 ;cylinder
	mov cl, 0x02 ;sector
	mov dh, 0x01 ;head
	call read_sector
	jmp kernel_offset
	
mbr db "You are in the MBR!",0
kernel_offset equ 0x1000

%include "utils/out.asm"
%include "utils/disk.asm"

times 510 - ($-$$) db 0
dw  0xaa55
