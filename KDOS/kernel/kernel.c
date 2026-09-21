#include "console-io.h"
#include "shell.h"

#define USER_INPUT_BUFFER_SIZE 1024

int userInputBufferIndex = 0;
char userInputBuffer[USER_INPUT_BUFFER_SIZE];

// This function will sit at the absolute top of the binary
void kernelMain(void) {

    clrscr();
    printString("Welcome to KDOS !\n");

    while (1) {

        char userInput = blockingInput();

        switch (userInput) {

        case (0x0D):
            
            if (userInputBufferIndex < USER_INPUT_BUFFER_SIZE) {
             
                userInputBuffer[userInputBufferIndex] = '\0';

            }

            userInputBufferIndex = 0;
            printChar('\n');
            printChar('\r');

            executeCommand(userInputBuffer);

            break;

        default:

            if (userInputBufferIndex < USER_INPUT_BUFFER_SIZE) {

                userInputBuffer[userInputBufferIndex] = userInput;
                userInputBufferIndex++;

            }

            printChar(userInput);

        }

    }

    __asm {

        halt:
        jmp halt;

    }

}
