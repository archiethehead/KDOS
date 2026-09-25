#include "disk.h"
#include "memory.h"
#include "console-io.h"

#define ROOT_DIRECTORY_SECTOR 33

sectorByte sectorBuffer[512 * SECTOR_BUFFER_SIZE] = { 0 };
diskAddressPacket kernelSectorBufferInformation;
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

void initRoot() {

    readSector(ROOT_DIRECTORY_SECTOR);
    currentDirectory = *((directory*)sectorBuffer);

}
