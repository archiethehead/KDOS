kernel_entry:
	loop:
		call binput
		cmp al, 'a'
		je print_a
		jmp loop

	print_a:
		mov si, msg
		call str_out
		jmp loop 

msg db "You just tapped A!",0

%define VERSION 0.0
%include "out.asm"
%include "disk.asm"
%include "in.asm"
