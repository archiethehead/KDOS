[org 0x0000]
%define VERSION '0.0'

kernel_entry:

	mov si, welcome_message
	call sout

	loop:
		
		call binput	
		call cout
		jmp loop
			


welcome_message db "Welcome to KDOS Version",VERSION,0

%include "in.asm"
%include "out.asm"
%include "disk.asm"
