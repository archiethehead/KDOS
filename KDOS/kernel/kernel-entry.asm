bits 16

global _cstart_
extern kernelMain_

section .text
_cstart_:

    mov ax, cs
    mov ds, ax
    mov es, ax
    xor bp, bp

    jmp kernelMain_
    jmp $

section .data