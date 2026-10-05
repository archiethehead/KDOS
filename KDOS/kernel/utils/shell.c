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
    {"cls", &clrscr},
    {"cd", &cd}

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

    char* line = "-------";

    newline();
    int len = strlen(currentDirectory.metadata.directoryName);
    int files = 0;
    int folders = 0;
    int executables = 0;

    line[len] = '\0';
    
    printString(line);

    newline();
    printString(currentDirectory.metadata.directoryName);
    newline();

    printString(line);

    line[len] = '-';

    newline();

    char fileCount = currentDirectory.metadata.fileCount;
    for (unsigned short i = 0; i < fileCount; i++) {

        char* gap = "          ";
        int len =  strlen(currentDirectory.entries[i].fileName);
        len = 10 - len;
        *(gap + len) = '\0';
        printString(currentDirectory.entries[i].fileName);

        char flag = currentDirectory.entries[i].fileSizeAndFlags >> 30;

        switch ((fileType)flag) {

        case (file):
            files++;
            break;

        case (folder):

            printString(gap);
            printString("<FOLDER>");
            folders++;

            break;

        case (executable):
            executables++;
            break;

        }

        *(gap + len) = ' ';
        newline();

    }

    printString("\n\n");

    printUint(files);
    printString(" File(s)\n");

    printUint(folders);
    printString(" Folder(s)\n");
    
    printUint(executables);
    printString(" Executable(s)\n");

}

void cd() {

    newline();

    if (argc < 2) {

        printString("No path was specified.\n");
        return;

    }

    if (argv[1][0] == '^') {

        bool isRoot = openParentDirectory();

        if (isRoot)
            printString("This folder is the root.\n");

        return;

    }

    uint64_t LBA;
    if (LBA = directoryExists(argv[1])) {

        changeDirectory(LBA);
        return;

    }

    printString("The folder/path specified does not exist.\n");

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
