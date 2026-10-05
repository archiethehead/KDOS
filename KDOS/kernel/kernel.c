#include "console-io.h"
#include "shell.h"
#include "disk.h"
#include "math.h"

#define USER_INPUT_BUFFER_SIZE 64

extern int KERNEL_ADDRESS;
int userInputBufferIndex = 0;
char userInputBuffer[USER_INPUT_BUFFER_SIZE];

// This function will sit at the absolute top of the binary
void kernelMain(void) {

    clrscr();
    printString("Welcome to KDOS !\n\n"); 
    initRoot();

    outputFilepath();
    printString(": ");
    
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
            newline();
            outputFilepath();
            printString(": ");
            break;
        
        case (0x08):
        
            if (userInputBufferIndex == 0)
                continue;

            vector2 cursorPosition = getCursorPos();
            cursorPosition.x--;
            setCursorPos(cursorPosition);
            userInputBuffer[--userInputBufferIndex] = ' ';
            printChar(' ');
            setCursorPos(cursorPosition);


        default:

            if (userInputBufferIndex < USER_INPUT_BUFFER_SIZE && userInput > 31 && userInput < 128) {

                userInputBuffer[userInputBufferIndex++] = userInput;
                printChar(userInput);

            }

        }

    }

    return;

}
