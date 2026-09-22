#ifndef CONSOLE_IO_H
#define CONSOLE_IO_H

void clrscr();

inline void printChar(char c) {

    __asm {

        mov ah, 0x0E
        mov al, c
        int 0x10

    }

}

inline void newline() {

    printChar('\n');
    printChar('\r');

}

void printString(char* string);
char blockingInput();

#endif // ifdef CONSOLE_IO_H
