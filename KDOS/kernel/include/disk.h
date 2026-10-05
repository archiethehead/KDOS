#ifndef DISK_H
#define DISK_H

#include "int.h"
#include "bool.h"

#define SECTOR_BUFFER_SIZE 0x0001
#define ACTIVE_DRIVE 4

typedef enum {	

	file 		= 0,
	folder 		= 1,
	executable 	= 2

} fileType;

typedef enum {

	flpOne	= 0,
	flpTwo 	= 1,
	hddOne	= 2,
	hddTwo	= 3

} driveNum;

typedef struct {

	char fileName[8];
	uint32_t diskSector;
	uint32_t fileSizeAndFlags;

} directoryEntry;

typedef struct {

	uint32_t size;

} fileTag;

typedef struct {
	
	char directoryName[8];
	uint16_t fileCount;
	uint32_t currentDir;
	uint32_t parentDir;
	uint32_t nextDir;

} directoryMetadata;

typedef struct {

	fileTag header;
	directoryMetadata metadata;
	directoryEntry entries[30];
	char padding[2];
	fileTag footer;

} directory ;

typedef struct {

	uint8_t sizeOfPacket;
	uint8_t reserved;
	uint16_t numberOfSectorsToTransfer;
	uint16_t bufferOffset;
	uint16_t segmentOffset;
	uint64_t logicalBaseAddress;

} diskAddressPacket;

typedef struct {

	uint8_t data;

} sectorByte;

typedef struct {

	driveNum numAndFlag;
	char symbol[4];

} driveSymbol;

extern uint8_t hardDriveCount;
extern uint8_t floppyDriveCount;
extern char filePathBuffer[];
extern diskAddressPacket kernelSectorBufferInformation;
extern directory currentDirectory;

uint64_t directoryExists(char* folderName);
bool openParentDirectory();
void changeDirectory(uint64_t LBA);
void outputFilepath();
void initRoot();
void initDrives();

#endif // ifdef DISK_H
