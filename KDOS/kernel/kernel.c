#include "conio.h"

char* CRLF = "\n\r";

void printChar(char c) {

    __asm {

        mov ah, 0x0E
        mov al, c
        int 0x10

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

    register unsigned short incrementCount = 0;

    while (*string != '\0') {

        printChar(*string);
        string++;
        incrementCount++;

    }

    string -= incrementCount;

}

// This function will sit at the absolute top of the binary
void kernelMain(void) {

    clrscr();

    while (1) {

        char userInput = blockingInput();

        switch (userInput) {

        case (0x0D):
            printString(CRLF);
            break;

        default:
            printChar(userInput);

        }

    }

    __asm {

        halt:
        jmp halt;

    }

}
