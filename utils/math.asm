BITS 16

clear_array:
	
	.loop:
		mov [bx], 0
		inc bx
		dec ax
		cmp ax, 0
		jne .loop

	
	ret	
