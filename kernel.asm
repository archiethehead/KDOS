[org 0x1000]
BITS 16
 
%define VERSION "0.0"
%define COMMAND_COUNT 2
%DEFINE CR 0x0d
%DEFINE LF 0x0a

kernel_entry:
	
	call clrscr
	
	cli

    mov ax, 0
	mov bx, ax
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
		inc bx

		jmp .shell_loop

		.enter_command:

			mov byte [si], 0
			call newline
			call is_command
			mov si, input_buffer
			mov byte [si], 0
			jmp .shell_loop

is_command:
	
	xor dx, dx
	mov di, shell_commands
	.loop:

		mov bx, [di]
		push di
		mov di, bx
		mov si, input_buffer
		call strcmp
		pop di
		cmp cx, 0
		je .found_command	
		add di, 2

		inc dx
		cmp dx, 2
		jne .loop
		ret

	.found_command:
		shl dx, 1
		mov bx, shell_command_function_pointers
		add bx, dx
		mov bx, [bx]
		jmp bx
		
			

shutdown:
	mov ax, 0x5307
	mov bx, 0x0001
	mov cx, 0x0003
	int 0x15

help:
	
	push si
	mov si, help_message
	call strout		
	pop si
	
	ret

%include "utils/array.asm"
%include "utils/out.asm"
%include "utils/in.asm"
%include "utils/disk.asm"

input_buffer times 255 db 0
welcome_message db "Welcome to KDOS Version ",VERSION, "!", 0
help_message db CR,LF,"COMMAND-LIST:",CR,LF,"EXIT --> Shuts down your machine.",CR,LF,"HELP --> Outputs a list of commands.",CR,LF,"Note: Commands are not case-sensitive.",0

shell_commands:
	dw exit_str
	dw help_str

exit_str db "exit",0
help_str db "help",0

shell_command_function_pointers:
	dw shutdown
	dw help
