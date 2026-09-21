#include "shell.h"
#include "console-io.h"
#include "memory.h"

const command shellCommands[] = {

    {"exit", &exit}

};

void exit() {

    __asm {

        mov ax, 0x5307
        mov bx, 0x0001
        mov cx, 0x0003
        int 0x15

    }

}

void executeCommand(const char* userInput) {

    for (int i = 0; i < 1; i++) {

        if (strequal(userInput, shellCommands[i].name)) {
         
            shellCommands[i].functionPointer();
            return;

        }

    }

    printString(userInput);
    printString(" is not a recognised command\n");

}