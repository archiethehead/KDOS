#include "console-io.h"
#include "shell.h"
#include "disk.h"
#include "memory.h"

#define MAX_ARGS 8

char* argv[MAX_ARGS];
uint8_t argc = 0;

const command shellCommands[] = {

    {"exit", &exit},
    {"sysinfo", &sysinfo},
    {"dir", &dir},
    {"cls", &clrscr}

};

void exit() {

    __asm {

        mov ax, 0x5307
        mov bx, 0x0001
        mov cx, 0x0003
        int 0x15

    }

}

void sysinfo() {

    newline();

    uint32_t memory = 1048576L; //1 MiB
    uint16_t oneKibBetween1and16Meg = 0;
    uint16_t sixtyFourKibBeyond16Meg = 0;

    __asm {

        mov ax, 0xE801
        int 0x15
        mov oneKibBetween1and16Meg, ax
        mov sixtyFourKibBeyond16Meg, bx

    }

    uint32_t ULoneKibBetween1and16Meg = (uint32_t)oneKibBetween1and16Meg * 1024UL;    // to bytes
    uint32_t ULsixtyFourKibBeyond16Meg = (uint32_t)sixtyFourKibBeyond16Meg * 65536UL; // to bytes

    memory += ULoneKibBetween1and16Meg;
    memory += ULsixtyFourKibBeyond16Meg;
    memory = memory >> 20; // x >> 20 == x / 1048576L

    char numberBuffer[32];
    printUint((uint64_t)memory);
    printString(numberBuffer);
    printString(" MiB of extended-memory\n");
     
    uint16_t conventionalMemory = 0;
    __asm {

        int 0x12
        mov conventionalMemory, ax
    
    }
    
    printUint(conventionalMemory);
    printString(" KiB of usable memory\n");
    printUint(hardDriveCount);
    printString(" Hard Drive(s) connected\n");
    printUint(floppyDriveCount);
    printString(" Floppy Drive(s) connected\n");

}

void dir() {

    newline();
    int len = strlen(currentDirectory.metadata.directoryName);
    int files = 0;
    int folders = 0;
    int executables = 0;

    for (int i = 0; i < len; i++) {

        printChar('-');

    }

    newline();
    printString(currentDirectory.metadata.directoryName);
    newline();

    for (int i = 0; i < len; i++) {

        printChar('-');

    }

    newline();

    char fileCount = currentDirectory.metadata.fileCount;
    for (unsigned short i = 0; i < fileCount; i++) {

        int len =  strlen(currentDirectory.entries[i].fileName);
        len = 10 - len;
        printString(currentDirectory.entries[i].fileName);

        char flag = currentDirectory.entries[i].fileSizeAndFlags >> 30;

        switch ((fileType)flag) {

        case (file):
            files++;
            break;

        case (folder):

            for (int j = 0; j < len; j++)
                printChar(' ');

            printString("<FOLDER>");
            folders++;

            break;

        case (executable):
            executables++;
            break;

        }

        newline();

    }

    newline();
    newline();

    printUint(files);
    printString(" File(s)");
    newline();

    printUint(folders);
    printString(" Folder(s)");
    newline();
    
    printUint(executables);
    printString(" Executable(s)");
    newline();

}

void cd() {



}

void executeCommand(char* userInput) {

    for (int i = 0; i < MAX_ARGS; i++)
        argv[i] = 0;
    argc = 0;

    char* currentArg = userInput;

    while (*currentArg != '\0') {

        while (*currentArg == ' ')
            currentArg++;

        if (*currentArg == '\0')
            break;
        
        if (argc >= MAX_ARGS)
            break;

        argv[argc++] = currentArg;
        
        while (*currentArg != ' ' && *currentArg != '\0')
            currentArg++;

        if (*currentArg == ' ') {
         
            *currentArg = '\0';
            currentArg++;

        }

    }

    for (int i = 0; i < COMMAND_COUNT; i++) {

        if (strequal(userInput, shellCommands[i].name)) {
         
            shellCommands[i].functionPointer();
            return;

        }

    }

    printString(userInput);
    printString(" is not a recognised command\n");

}
