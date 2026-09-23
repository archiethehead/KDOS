#include "console-io.h"
#include "shell.h"
#include "disk.h"

#define USER_INPUT_BUFFER_SIZE 64

extern int KERNEL_ADDRESS;
int userInputBufferIndex = 0;
char userInputBuffer[USER_INPUT_BUFFER_SIZE];

// This function will sit at the absolute top of the binary
void kernelMain(void) {

    clrscr();
    printString("Welcome to KDOS !\n");

    readFolder(33);

    while (1) {

        char userInput = blockingInput();

        switch (userInput) {

        case (0x0D):
            
            if (userInputBufferIndex < USER_INPUT_BUFFER_SIZE) {
             
                userInputBuffer[userInputBufferIndex] = '\0';

            }

            userInputBufferIndex = 0;
            newline();
            executeCommand(userInputBuffer);

            break;

        default:

            if (userInputBufferIndex < USER_INPUT_BUFFER_SIZE) {

                userInputBuffer[userInputBufferIndex] = userInput;
                userInputBufferIndex++;
                printChar(userInput);

            }

        }

    }

    return;

}