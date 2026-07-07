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

	mov si, input_buffer

	.shell_loop:
		call binput
		
		cmp al, 0x0d
		je .enter_command
		
		call chout
		mov [si], al
		inc si
		inc ax

		jmp .shell_loop

		.enter_command:
			mov si, input_buffer
			call strout
			call newline
			call is_command
			mov si, input_buffer 
			call clear_array
			mov si, input_buffer
			jmp .shell_loop

is_command:
	mov si, input_buffer
	mov di, exit
	call strcmp
	cmp cx, 0
	je shutdown
	ret	

shutdown:
	mov ax, 0x5307
	mov bx, 0x0001
	mov cx, 0x0003
	int 0x15

%include "utils/array.asm"
%include "utils/out.asm"
%include "utils/in.asm"
%include "utils/disk.asm"
%define VERSION "0.0"

input_buffer times 255 db 0
welcome_message db "Welcome to KDOS Version ",VERSION, "!", 0

shell_commands:
	exit db "exit",0
	help db "help",0

command_count db 2
