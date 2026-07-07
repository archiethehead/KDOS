BITS 16

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
