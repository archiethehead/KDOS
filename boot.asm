[org 0x7c00]

boot:
	xor ax, ax
	mov ds, ax
	mov es, ax

	mov si, hello_world
	call str_out

	mov ax, 0x0000
	mov es, ax
	mov bx, kernel_offset

	mov dl, 0x80
	mov ch, 0x00
	mov cl, 0x02
	mov dh, 0x00
	call read_sector
	jmp kernel_offset
		

hello_world db "Hello World",0
kernel_offset equ 0x1000

%include "disk.asm"
%include "out.asm"

times 510 - ($-$$) db 0
dw  0xaa55
