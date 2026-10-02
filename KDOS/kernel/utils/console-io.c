#include "console-io.h"
#include "memory.h"

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

void printInt(int N) {

    char numbuff[32];
    intToStr((int64_t)N, numbuff);
    printString(numbuff);

}

void cdecl formout(char* format, ...) {

    char formoutBuffer[FORMOUT_BUFFER_SIZE];
    char* firstArg = (char*)&format + sizeof(format);
    uint16_t index = 0;
    
    while (*format != '\0' && index < FORMOUT_BUFFER_SIZE) {

        if (*format == '%') {

            switch(*(++format)) {

            case 'u':
                break;


            }

        }

        else {

            formoutBuffer[index++] = *format++;

        }

    }

    formoutBuffer[index] = '\0';

}
