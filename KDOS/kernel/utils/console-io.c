#include "console-io.h"
#include "memory.h"

char formoutBuffer[FORMOUT_BUFFER_SIZE];

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

void printString(const char* string) {

    while (*string != '\0') {

        if (*string == '\n')
            printChar('\r');

        printChar(*string);
        string++;

    }

}

void printInt(int64_t N) {

    char numbuff[32];
    intToStr(N, numbuff);
    printString(numbuff);

}

void printUint(uint64_t N) {

    char numbuff[32];
    uintToStr(N, numbuff);
    printString(numbuff);

}

vector2 getCursorPos() {

    uint8_t cursorPosX = 0;
    uint8_t cursorPosY = 0;

    __asm {

        mov ah, 0x03
        int 0x10

        mov cursorPosX, dl
        mov cursorPosY, dh

    }

    vector2 cursorPosition;
    cursorPosition.x = cursorPosX;
    cursorPosition.y = cursorPosY;

    return cursorPosition;

}

void setCursorPos(vector2 newPosition) {

    uint8_t x = newPosition.x;
    uint8_t y = newPosition.y;

    __asm {

        mov ah, 0x02
        mov bh, 0x00
        mov dl, x
        mov dh, y
        int 0x10

    }

}
