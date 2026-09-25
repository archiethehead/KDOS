#ifndef CONSOLE_IO_H
#define CONSOLE_IO_H

void clrscr();

void printString(const char* string);

inline void printChar(char c) {

    __asm {

        mov ah, 0x0E
        mov al, c
        int 0x10

    }

}

inline void newline() {

    printString("\n");

}

char blockingInput();

#endif // ifdef CONSOLE_IO_H
