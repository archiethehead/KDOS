#ifndef CONSOLE_IO_H
#define CONSOLE_IO_H

#define FORMOUT_BUFFER_SIZE 512

#include "int.h"
#include "math.h"
#include "time.h"

void clrscr();

void printString(const char* string);
void printHugeString(const char huge* string);

void printInt(int64_t N);
void printUint(uint64_t N);

void printTime(uint32_t systemTicks);

inline void printChar(char c) {

    __asm {

        push ax

        mov ah, 0x0E
        mov al, c
        int 0x10

        pop ax

    }

}

inline void newline() {

    printString("\n");

}

char blockingInput();

vector2 getCursorPos();
void setCursorPos(vector2 newPosition);

#endif // ifdef CONSOLE_IO_H
