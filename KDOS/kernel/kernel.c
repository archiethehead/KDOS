#include "console-io.h"
#include "bool.h"
#include "shell.h"
#include "disk.h"
#include "math.h"
#include "video.h"
#include "time.h"
#include "logo.h"

#define USER_INPUT_BUFFER_SIZE 64

int userInputBufferIndex = 0;
char userInputBuffer[USER_INPUT_BUFFER_SIZE];

void kernelMain() {

    __asm {

        mov ah, 0x00
        mov al, 0x13
        int 0x10

    }

    setVideoMode(VGA);
    drawXBM(KDOS_width, KDOS_height, KDOS_bits);
    initRoot();
    wait(3);

    setVideoMode(text);
    clrscr();
    loadFile(37);
    printString("Welcome to KDOS !\n\n"); 

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
            break;


        default:

            if (userInputBufferIndex < USER_INPUT_BUFFER_SIZE && userInput > 31 && userInput < 128) {

                userInputBuffer[userInputBufferIndex++] = userInput;
                printChar(userInput);

            }

        }

    }

    return;

}
