#ifndef DISK_H
#define DISK_H

#define SECTOR_BUFFER_SIZE 0x0001

typedef enum {	

	file = 0,
	folder = 1,
	executable = 2

} fileType;

typedef struct {

	char fileName[8];
	unsigned long diskSector;
	unsigned long fileSizeAndFlags;

} directoryEntry;

typedef struct {

	unsigned long size;

} fileTag;

typedef struct {
	
	char directoryName[8];
	unsigned short fileCount;
	unsigned long currentDir;
	unsigned long parentDir;
	unsigned long nextDir;

} directoryMetadata;

typedef struct {

	fileTag header;
	directoryMetadata metadata;
	directoryEntry entries[30];
	char padding[2];
	fileTag footer;

} directory ;

typedef struct {

	unsigned char sizeOfPacket;
	unsigned char reserved;
	unsigned short numberOfSectorsToTransfer;
	unsigned short bufferOffset;
	unsigned short segmentOffset;
	unsigned long long logicalBaseAddress;

} diskAddressPacket;

typedef struct {

	unsigned char data;

} sectorByte;

extern char filePathBuffer[];
extern diskAddressPacket kernelSectorBufferInformation;
extern directory  currentDirectory;

void initRoot();

#endif // ifdef DISK_H
