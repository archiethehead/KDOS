#include "disk.h"
#include "memory.h"
#include "console-io.h"
#include "bool.h"

#define ROOT_DIRECTORY_SECTOR 33
#define MAX_NESTED_DIRECTORY 16

uint8_t sectorBuffer[512 * SECTOR_BUFFER_SIZE] = { 0 };
diskAddressPacket kernelSectorBufferInformation;

uint8_t hardDriveCount;
uint8_t floppyDriveCount;
uint8_t directoryBufferIndex = 0;
uint32_t directoryBuffer[MAX_NESTED_DIRECTORY];
bool isDirectoryChanged = false;
char directoryPath[MAX_NESTED_DIRECTORY][8];
directory currentDirectory;

driveSymbol driveSymbols[] = {

    {flpOne, "~I"},
    {flpTwo, "~II"},
    {hddOne, "#I"},
    {hddTwo, "#II"}

};

diskAddressPacket kernelSectorBufferInformation = {

    0x10,
    0x00,
    SECTOR_BUFFER_SIZE,
    (uint16_t)&sectorBuffer,
    0x1000,
    0x00000000

};

void readSector(uint64_t LBA) {

    uint16_t kernelSectorBufferInformationAddress = (uint16_t)&kernelSectorBufferInformation;
    kernelSectorBufferInformation.logicalBaseAddress = LBA;
    
    __asm {

        mov ah, 0x42
        mov dl, 0x80
        mov si, kernelSectorBufferInformationAddress
        int 0x13

    }

}

static void createFilePath() {

    char* directoryName;
    uint16_t directoryCount = 0;
    directoryMetadata metadataBuffer = currentDirectory.metadata;

    do {

        directoryName = currentDirectory.metadata.directoryName;

        strcpy(directoryPath[directoryCount], (sizeof(directoryPath)  / MAX_NESTED_DIRECTORY), directoryName);

        readSector(currentDirectory.metadata.parentDir);
        directoryCount++;

    } while (!((currentDirectory.metadata = ((directory*)sectorBuffer)->metadata).parentDir) && directoryCount < MAX_NESTED_DIRECTORY);

    currentDirectory.metadata = metadataBuffer;
    directoryBufferIndex = directoryCount;
    isDirectoryChanged = false;

}

void outputFilepath() {

    if (isDirectoryChanged)
        createFilePath();

    for (int i = 0; i < directoryBufferIndex; i++) {

        printString(directoryPath[(directoryBufferIndex - i) - 1]);
        
        if (directoryBufferIndex - i != 1)
            printChar('.');

    }

}

void initRoot() {

    initDrives();
    readSector(ROOT_DIRECTORY_SECTOR);
    memcopy(&currentDirectory, sizeof(currentDirectory), sizeof(sectorBuffer), &sectorBuffer);
    directoryBuffer[directoryBufferIndex++] = currentDirectory.metadata.currentDir;
    createFilePath();

}

uint8_t getHardDriveCount() {

    uint8_t res = 0;
    
    __asm {

        push ax
        push bx
        push cx
        push dx
        push es
        push si
        push di

        mov dl, 0x80
        mov ah, 0x08
        xor di, di
        mov es, di
        int 0x13
        jc .doesnt_exist
        mov res, dl

        .doesnt_exist:
        pop es
        pop dx
        pop cx
        pop bx
        pop ax

    }

    return res;

}

uint8_t getFloppyDriveCount() {

    uint8_t res = 0;

    __asm {

        int 0x11
        test ax, 1
        jz .doesnt_exist
        
        mov cl, 6
        shr ax, cl
        and ax, 0x03
        add ax, 1
        mov res, al

        .doesnt_exist:

    }

    return res;

}

void initDrives() {

    floppyDriveCount = getFloppyDriveCount();
    hardDriveCount = getHardDriveCount();

    for (int i = 0; i < floppyDriveCount; i++) {

        driveSymbols[i].numAndFlag |= true;

    }

    for (int i = 0; i < hardDriveCount; i++) {

        driveSymbols[i + 2].numAndFlag |= true;

    }

}
