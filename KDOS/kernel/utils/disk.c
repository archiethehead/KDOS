#include "disk.h"
#include "memory.h"
#include "console-io.h"
#include "bool.h"

#define ROOT_DIRECTORY_SECTOR 33
#define MAX_NESTED_DIRECTORY 16

uint8_t sectorBuffer[512 * SECTOR_BUFFER_SIZE] = { 0 };
directory currentDirectory;
diskAddressPacket kernelSectorBufferInformation;
driveSymbol currentDrive;

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

uint8_t hardDriveCount;
uint8_t floppyDriveCount;
uint8_t directoryBufferIndex = 0;
uint32_t directoryBuffer[MAX_NESTED_DIRECTORY];

bool isDirectoryChanged = false;
char directoryPath[MAX_NESTED_DIRECTORY][8];

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

void loadDirectory(uint64_t LBA) {

    readSector(LBA);
    memcopy(&currentDirectory, sizeof(currentDirectory), sizeof(sectorBuffer), &sectorBuffer);
    directoryBuffer[directoryBufferIndex++] = currentDirectory.metadata.currentDir;
    isDirectoryChanged = true;

}

static void createFilePath() {

    char* directoryName;
    uint16_t directoryCount = 0;
    directoryMetadata metadataBuffer = currentDirectory.metadata;

    while (directoryCount < MAX_NESTED_DIRECTORY) {

        directoryName = currentDirectory.metadata.directoryName;
        strcpy(directoryPath[directoryCount], (sizeof(directoryPath)  / MAX_NESTED_DIRECTORY), directoryName);
        directoryCount++;

        if (!currentDirectory.metadata.parentDir)
            break;
        
        readSector(currentDirectory.metadata.parentDir);
        currentDirectory.metadata = ((directory*)sectorBuffer)->metadata;

    }

    currentDirectory.metadata = metadataBuffer;
    directoryBufferIndex = directoryCount;
    isDirectoryChanged = false;

}

void outputFilepath() {

    if (isDirectoryChanged)
        createFilePath();

    printString(currentDrive.symbol);
    printChar('.');

    for (int i = 0; i < directoryBufferIndex; i++) {

        printString(directoryPath[(directoryBufferIndex - i) - 1]);
        
        if (directoryBufferIndex - i != 1)
            printChar('.');

    }

}

void initRoot() {

    currentDrive = driveSymbols[2];
    initDrives();
    loadDirectory(ROOT_DIRECTORY_SECTOR);

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

        driveSymbols[i].numAndFlag |= ACTIVE_DRIVE;

    }

    for (int i = 0; i < hardDriveCount; i++) {

        driveSymbols[i + 2].numAndFlag |= ACTIVE_DRIVE;

    }

}

bool openDirectory(char* folderName) {

    for (uint16_t i =  0; i < currentDirectory.metadata.fileCount; i++) {     

        if (strequal(currentDirectory.entries[i].fileName, folderName) && FILE_TYPE(currentDirectory.entries[i]) == folder) {
                loadDirectory(currentDirectory.entries[i].diskSector);
                return true;

        }

    }

    return false;

}

bool openParentDirectory() {

    bool isRoot = true;

    if (currentDirectory.metadata.parentDir == 0)
        return isRoot;
    
    loadDirectory(currentDirectory.metadata.parentDir);
    directoryBufferIndex--;
    return isRoot = false;

}

