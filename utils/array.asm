BITS 16

clear_array:

	.loop:
		mov [si], 0
		inc si
		dec ax
		cmp ax, 0
		jne .loop

	ret

strcmp:

	.loop:		
	
		mov al, [si]
		cmp al, [di]
		jne .ne
	
		cmp al, 0x00
		je .e

		inc si
		inc di
		jmp .loop	

	.e:
		mov cx, 0
		ret		

	.ne:
		mov cx, 1
		ret
