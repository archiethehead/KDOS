#include "disk.h"
#include "memory.h"
#include "console-io.h"

#define ROOT_DIRECTORY_SECTOR 33

sectorByte sectorBuffer[512 * SECTOR_BUFFER_SIZE] = { 0 };
diskAddressPacket kernelSectorBufferInformation;

directory currentDirectory;
char filePathBuffer[256] = { 0 };
int filePathBufferIndex = 0;
unsigned long directoryBuffer[30];
unsigned char directoryBufferIndex = 0;

diskAddressPacket kernelSectorBufferInformation = {

    0x10,
    0x00,
    SECTOR_BUFFER_SIZE,
    (unsigned short)&sectorBuffer,
    0x1000,
    0x00000000

};

void readSector(unsigned long long LBA) {

    unsigned short kernelSectorBufferInformationAddress = (unsigned short)&kernelSectorBufferInformation;
    kernelSectorBufferInformation.logicalBaseAddress = LBA;
    
    __asm {

        mov ah, 0x42
        mov dl, 0x80
        mov si, kernelSectorBufferInformationAddress
        int 0x13

    }

}

void writeToFilePathBuffer(char* directoryName) {

    if (filePathBufferIndex >= sizeof(filePathBuffer) || sizeof(filePathBuffer) - filePathBufferIndex <= strlen(directoryName)) {

        filePathBufferIndex += strlen(directoryName);
        printString(filePathBuffer);
        newline();
        char numbuff[10];
        intToStr(filePathBufferIndex, numbuff);
        printString(numbuff);
        return;

    }

    strcpy(filePathBuffer + filePathBufferIndex, sizeof(filePathBuffer) - filePathBufferIndex, directoryName);
    filePathBufferIndex += strlen(directoryName) + 1;
    filePathBuffer[--filePathBufferIndex] = '\\';
    filePathBufferIndex++;
    printString(filePathBuffer);
    newline();

    char numbuff[10];
    intToStr(filePathBufferIndex, numbuff);
    printString(numbuff);

    newline();

}

void eraseFromFilePathBuffer(const char* directoryName) {
    
    if (filePathBufferIndex > sizeof(filePathBuffer))
        return;

    chrcpy(filePathBuffer + (filePathBufferIndex - strlen(directoryName)) - 1, '\0', strlen(directoryName) + 1);
    printString(filePathBuffer);
    newline();

    char numbuff[10];
    intToStr(filePathBufferIndex, numbuff);
    printString(numbuff);

    newline();

    filePathBufferIndex -= strlen(directoryName) + 1;

}

void initRoot() {

    readSector(ROOT_DIRECTORY_SECTOR);
    currentDirectory = *((directory*)sectorBuffer);
    directoryBuffer[directoryBufferIndex++] = currentDirectory.metadata.currentDir;
    
    for (int i = 0; i < 30; i++) {

        writeToFilePathBuffer(currentDirectory.metadata.directoryName);
        blockingInput();

    }

        for (int i = 0; i < 30; i++) {

        eraseFromFilePathBuffer(currentDirectory.metadata.directoryName);
        blockingInput();

    }

}
