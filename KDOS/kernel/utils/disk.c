#include "disk.h"
#include "memory.h"
#include "console-io.h"

#define ROOT_DIRECTORY_SECTOR 33

sectorByte sectorBuffer[512 * SECTOR_BUFFER_SIZE] = { 0 };
diskAddressPacket kernelSectorBufferInformation;

unsigned char hardDriveCount;
unsigned char floppyDriveCount;
unsigned long directoryBuffer[30];
unsigned char directoryBufferIndex = 0;
directory currentDirectory;

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

void outputFilepath() {

    directoryMetadata metadataBuffer = currentDirectory.metadata;

    char* directoryName = currentDirectory.metadata.directoryName;
    int directoryCount = 1;

    __asm {

        push directoryName

    }

    while (currentDirectory.metadata.parentDir != 0) {

        readSector(currentDirectory.metadata.parentDir);
        currentDirectory.metadata = ((directory*)sectorBuffer)->metadata;
        directoryName = currentDirectory.metadata.directoryName;

        __asm {

            push directoryName

        }

        directoryCount++;

    }

    for (int i = 0; i < directoryCount; i++) {

        __asm {

            pop directoryName

        }

        printString(directoryName);
        
        if (directoryCount - i != 1)
            printChar('.');

    }

    currentDirectory.metadata = metadataBuffer;

}

void initRoot() {

    initDrives();
    readSector(ROOT_DIRECTORY_SECTOR);
    currentDirectory = *((directory*)sectorBuffer);
    currentDirectory.metadata.parentDir = 0x0000;
    directoryBuffer[directoryBufferIndex++] = currentDirectory.metadata.currentDir;

}

unsigned char getHardDriveCount() {

    unsigned char res = 0;
    
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

unsigned char getFloppyDriveCount() {

    unsigned char res = 0;

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

    hardDriveCount = getHardDriveCount();
    floppyDriveCount = getFloppyDriveCount();

}
