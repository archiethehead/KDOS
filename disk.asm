BITS 16

read_sector:
	mov ah, 0x02
	mov al, 0x01
	int 0x13
	jc read_fail
	
	mov si, success_message
	call str_out
	ret

read_fail:
	mov si, error_message
	call str_out
	jmp $

success_message db "SUCCESS --> Read disk :)",0
error_message db "ERROR --> Could not read disk :(",0
