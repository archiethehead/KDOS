#include "console-io.h"

// This function will sit at the absolute top of the binary
void kernelMain(void) {

    clrscr();
    printString("Welcome to KDOS !\n");

    while (1) {

        char userInput = blockingInput();

        switch (userInput) {

        case (0x0D):
            printChar('\n');
            printChar('\r');

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
