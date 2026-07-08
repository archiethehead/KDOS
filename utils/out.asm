BITS 16

%DEFINE CR 0x0d
%DEFINE LF 0x0a

newline:
	mov al, CR
	call chout
	mov al, LF
	call chout
	ret

clrscr:
	mov ah, 0x00
	mov al, 0x03
	int 0x10
	ret

chout:
	mov ah, 0x0e
	int 0x10
	ret

strout:
	lodsb
	cmp al, 0
	je .done
	call chout
	jmp strout

	.done:
		cmp dx, 1
		je .return
		call newline
	
	.return:
		ret

numerical_buffer times 50 db 0
