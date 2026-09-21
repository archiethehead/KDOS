#include "console-io.h"

void clrscr() {

    __asm {

        push es
        push cx
        push di
        push ax

        mov ax, 0x0B800
        mov es, ax
        xor di, di

        mov cx, 2000
        mov ah, 0x07
        mov al, ' '

        rep stosw

        mov ah, 0x02
        mov bh, 0x00
        mov dh, 0x00
        mov dl, 0x00
        int 0x10

        pop ax
        pop di
        pop cx
        pop es

    }

}

char blockingInput() {

    char input = 0;

    __asm {

        mov ah, 0x00
        int 0x16
        mov input, al
        
    }

    return input;

}

void printString(char* string) {

    while (*string != '\0') {

        if (*string == '\n')
            printChar('\r');

        printChar(*string);
        string++;

    }

}
