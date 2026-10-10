bits 16

global _cstart_
global _version_type
global _version_major
global _version_minor
global _version_patch
global _version_name
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
    _version_type db "Alpha",0
    _version_major db 1
    _version_minor db 0
    _version_patch db 0
    _version_name db "ArchOS",0