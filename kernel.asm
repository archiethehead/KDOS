[org 0x1000]
BITS 16

kernel_entry:
	
	call clrscr
	
	cli

    mov ax, 0
    mov ss, ax          
    mov sp, 0xffff

	sti

	mov si, welcome_message
	call strout
	call newline

	mov ax, 0	
	mov bx, input_buffer	

	.loop:
		call binput
		
		cmp al, 0x0d
		je .enter_command
		
		call chout
		mov [bx], al
		inc bx
		inc ax

		jmp .loop

	.enter_command:
	
		call newline
		mov bx, input_buffer
		call clear_array
		mov bx, input_buffer
		jmp .loop

%include "utils/math.asm"
%include "utils/out.asm"
%include "utils/in.asm"
%include "utils/disk.asm"
%define VERSION "0.0"

input_buffer times 255 db 0
welcome_message db "Welcome to KDOS Version ",VERSION, "!", 0
