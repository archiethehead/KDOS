#include "console-io.h"
#include "shell.h"
#include "disk.h"
#include "memory.h"

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

    unsigned long memory = 1048576L; //1 MiB
    unsigned short oneKibBetween1and16Meg = 0;
    unsigned short sixtyFourKibBeyond16Meg = 0;

    __asm {

        mov ax, 0xE801
        int 0x15
        mov oneKibBetween1and16Meg, ax
        mov sixtyFourKibBeyond16Meg, bx

    }

    unsigned long ULoneKibBetween1and16Meg = (unsigned long)oneKibBetween1and16Meg * 1024UL;    // to bytes
    unsigned long ULsixtyFourKibBeyond16Meg = (unsigned long)sixtyFourKibBeyond16Meg * 65536UL; // to bytes

    memory += ULoneKibBetween1and16Meg;
    memory += ULsixtyFourKibBeyond16Meg;
    memory = memory >> 20; // x >> 20 == x / 1048576L

    char numberBuffer[16];
    ulongToStr(memory, numberBuffer);
    printString(numberBuffer);
    printString(" MiB of extended-memory\n");

     
    int conventionalMemory = 0;
    __asm {

        int 0x12
        mov conventionalMemory, ax
    
    }
    
    intToStr(conventionalMemory, numberBuffer);
    printString(numberBuffer);
    printString(" KiB of usable memory\n");

    char numberOfConnectedDrives = 0;

    __asm {

        xor ax, ax
        mov es, ax
        mov di, ax

        mov ah, 0x08
        mov dl, 0x80
        int 0x13

        mov numberOfConnectedDrives, dl

    }

    intToStr(numberOfConnectedDrives, numberBuffer);
    printString(numberBuffer);
    printString(" drive(s) connected\n");

}

void dir() {

    printString("\n\n");
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

    char fileCount = currentDirectory.metadata.fileCount;
    for (unsigned short i = 0; i < fileCount; i++) {

        printString(currentDirectory.entries[i].fileName);
        newline();

        char flag = currentDirectory.entries[i].fileSizeAndFlags >> 30;

        switch ((fileType)flag) {

        case (file):
            files++;

        case (folder):
            folders++;

        case (executable):
            executables++;

        }

    }

    char numbuff[32];

    newline();
    newline();
    printString("Files = ");
    intToStr(files, numbuff);
    printString(numbuff);
    newline();

    printString("Folders = ");
    intToStr(files, numbuff);
    printString(numbuff);
    newline();
    
    printString("Executables = ");
    intToStr(files, numbuff);
    printString(numbuff);
    newline();

}

void executeCommand(const char* userInput) {

    for (int i = 0; i < COMMAND_COUNT; i++) {

        if (strequal(userInput, shellCommands[i].name)) {
         
            shellCommands[i].functionPointer();
            return;

        }

    }

    printString(userInput);
    printString(" is not a recognised command\n");

}
