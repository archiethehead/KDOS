BITS 16

%DEFINE CR 0x0d
%DEFINE LF 0x0a

char_out:
	mov ah, 0x0e
	int 0x10
	ret	

str_out:
	lodsb
	cmp al, 0
	je done
	call char_out
	jmp str_out

	done:
		mov al, CR
		call char_out
		mov al, LF
		call char_out
		ret

