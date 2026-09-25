bits 16

global _cstart_
global _KERNEL_ADDRESS
extern kernelMain_

section .text
_cstart_:

    mov [_KERNEL_ADDRESS], ax

    mov ax, cs
    mov ds, ax
    mov es, ax

    jmp kernelMain_
    jmp $

section .data
_KERNEL_ADDRESS db 0x0