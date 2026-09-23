#include "disk.h"
#include "memory.h"
#include "console-io.h"

sectorByte sectorBuffer[512 * SECTOR_BUFFER_SIZE] = { 0 };

diskAddressPacket kernelSectorBufferInformation = {

    0x10,
    0x00,
    SECTOR_BUFFER_SIZE,
    (unsigned short)&sectorBuffer,
    0x1000,
    0x00000000

};

void readFolder(unsigned long long LBA) {

    char tempstr[10];
    int temp = (int)sectorBuffer[0].data;
    intToStr(temp, tempstr);
    printString(tempstr);
    unsigned short kernelSectorBufferInformationAddress = (unsigned short)&kernelSectorBufferInformation;

    kernelSectorBufferInformation.logicalBaseAddress = LBA;
    __asm {

        mov ah, 0x42
        mov dl, 0x80
        mov si, kernelSectorBufferInformationAddress
        int 0x13

    }

    temp = (int)sectorBuffer[0].data;
    intToStr(temp, tempstr);
    printString(tempstr);
    
}
