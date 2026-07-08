[org 0x1000]
BITS 16
 
%define VERSION "0.0"
%define COMMAND_COUNT 4
%DEFINE CR 0x0d
%DEFINE LF 0x0a

kernel_entry:
	
	call clrscr
	
	cli

    mov ax, 0
	mov bx, ax
	mov cx, ax
    mov ss, ax          
    mov sp, 0xffff

	sti

	mov si, welcome_message
	mov dx, 0
	call strout

	mov si, input_buffer

	.shell_loop:

		call binput
		
		cmp al, 0x0d
		je .enter_command
		
		call chout
		mov [si], al
		inc si

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
		cmp dx, COMMAND_COUNT
		jne .loop
		jmp cmd_not_recognised

	.found_command:
		shl dx, 1
		mov bx, shell_command_function_pointers
		add bx, dx
		mov bx, [bx]
		jmp bx
		
sysinfo:

	mov dx, 1
	mov si, memory
	call strout
	mov ah, 0x88
	int 0x15
	call itoa
	mov si, di
	mov dx, 1
	call strout
	xor dx, dx
	mov si, kb
	call strout
	ret
	
shutdown:
	mov ax, 0x5307
	mov bx, 0x0001
	mov cx, 0x0003
	int 0x15

help:
	push si
	mov si, help_message
	xor dx, dx
	call strout		
	pop si	
	ret

echo:
	mov si, input_buffer
	xor dx, dx
	call strout
	ret

cmd_not_recognised:
	mov dx, 1 
	mov si, error
	call strout
	mov si, input_buffer
	call strout
	xor dx, dx
	mov si, command_not_recognised
	call strout
	ret
	

%include "utils/math.asm"
%include "utils/array.asm"
%include "utils/out.asm"
%include "utils/in.asm"
%include "utils/disk.asm"
%include "utils/sysstrings.asm"
