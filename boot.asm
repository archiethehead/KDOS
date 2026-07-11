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
	mov ss, ax
    mov sp, 0x7c00
	mov ax, 0x0000
	mov es, ax
	mov bx, sector_offset
	call find_kernel
	
	cmp al, 1
 

	.kernel_not_found:
		jmp $

find_kernel:

	mov cl, 0x02	
	.loop:

		push cx

		call read_sector
		call cycle_sector

		pop cx
		
		cmp al, 1
		je .done

		inc cl
		mov [sector], cl
		cmp cl, 16
		jne .loop

	.done:
		ret	
	
read_sector:	
	
    .read_loop:
        mov ax, 0x0000
        mov es, ax
        mov dl, 0x00 ; drive number
        mov ch, 0x00 ; Track
		mov [track], ch
        mov dh, 0x01 ; head
		mov [head], dh
        mov ah, 0x02
        mov al, 0x01 ; number of sectors to be read
        mov bx, sector_offset
        int 0x13
        jc .floppy_timeout
		push si
       	mov si, disk_timeout
		mov [si], 0
		pop si
		ret

	; Standard floppy's can submit a read fail due to
	; needing to speed up first, and failing to do so
	; quick enough. For this, we give the floppy three
	; chances if it returns a read fail, and assume it's
	; a genuine error if it still fails.
	;
	; |
	; |
	; V

    .floppy_timeout:
       	push si
		mov si, disk_timeout
		add [si], 1
		cmp [si], 3
		pop si
        je .read_fail
        jmp .read_loop
    
    .read_fail:
	    mov si, disk_error
	    call strout
	    jmp $

cycle_sector:
	
	mov bx, sector_offset
	xor dx, dx
	
	.loop:

		mov cx, 11
		mov si, kern_name
		mov di, bx
		repe cmpsb	
		je .found
	
	.next_file:

		inc dx
		cmp dx, 16
		je .not_found
		add bx, 32
		jmp .loop
	
	.found:
		mov si, loading
		call strout
		mov al, 1
		ret
	
	.not_found:
		mov ax, 0
		ret			

; Linear Byte Address --> Cylinder-Head Sector
;
; |
; |
; V

lba2chs:
	xor dx, dx
	div word [BPB_SecPerTrk]
	inc dl
	mov byte [sector], dl
	xor dx, dx
	div word [BPB_NumHeads]
	mov byte [head], dl
	mov byte [track], al

mbr db "You are in the MBR!",0
kern_name db "KERNEL  BIN",0
loading db "Loading kernel.bin . . .",0
disk_error db "ERROR --> Could not read disk :(",0

sector db 0x00
head db 0x00
track db 0x00

sector_offset equ 0x7e00
kernel_offset equ 0x1000

disk_timeout db 0

%include "utils/out.asm"

times 510 - ($-$$) db 0
dw 0xaa55
