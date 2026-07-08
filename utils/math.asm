BITS 16

itoa:
	
	xor dx, dx
	xor cx, cx
	mov bx, 10

	.loop:
		xor dx, dx
		div bx
		add dx, 48
		push dx
		inc cx
		cmp ax, 0
		jne .loop

	mov di, itoa_buffer
	.poploop:
		pop dx
		mov [di], dl
		inc di
		dec cx
		cmp cx, 0
		jne .poploop
	mov di, itoa_buffer
	ret
	
		

itoa_buffer times 50 db 0
