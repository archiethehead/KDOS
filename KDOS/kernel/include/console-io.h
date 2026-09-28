#ifndef CONSOLE_IO_H
#define CONSOLE_IO_H

#define FORMOUT_BUFFER_SIZE 512

void clrscr();

void printString(const char* string);

void printInt(int N);

void cdecl formout(char* format, ...);

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
