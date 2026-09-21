bits 16

global _cstart_
extern kernelMain_

section .text
_cstart_:
    mov ax, cs
    mov ds, ax
    mov es, ax

    call kernelMain_
    jmp $