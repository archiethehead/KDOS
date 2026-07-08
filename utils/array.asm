BITS 16

strcmp:
	
	xor ax, ax
	
	.loop:		
	
		mov al, [si]
		cmp al, [di]
		jne .ne
	
		cmp al, 0
		je .e
		cmp al, 32
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
