BITS 16

read_sector:	
	mov ah, 0x02
	mov al, 0x01 ; number of sectors to read
	int 0x13
	jc read_fail
	ret

read_fail:
	jmp $
