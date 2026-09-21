mov ah, 0x0E
mov al, 'a'
int 0x10
jmp $

times 16384 - ($-$$) db 0