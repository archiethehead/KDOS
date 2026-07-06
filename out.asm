BITS 16

%DEFINE CR 0x0d
%DEFINE LF 0x0a

clrscr:
	mov ah, 0x00
	mov al, 0x03
	int 0x10

cout:

	cmp al, CR
	jne out
	mov al, LF
	call cout
	mov al, CR

	out:
		mov ah, 0x0e
		int 0x10
		ret	

sout:
	lodsb
	cmp al, 0
	je done
	call cout
	jmp sout

	done:
		mov al, CR
		call cout
		ret

