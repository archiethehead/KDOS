#ifndef DISK_H
#define DISK_H

#define SECTOR_BUFFER_SIZE 0x0001

typedef struct {

	char FileName[8];
	unsigned long DiskSector;
	unsigned long FileSizeAndFlags;

} DirectoryEntry;

typedef struct {

	unsigned long Size;

} FileTag;

typedef struct {
	
	char DirectoryName[8];
	unsigned short FileCount;
	unsigned long CurrentDir;
	unsigned long ParentDir;
	unsigned long NextDir;

} DirectoryMetada;

typedef struct {

	FileTag Header;
	DirectoryMetada Metadata;
	DirectoryEntry Entries[30];
	char padding[2];
	FileTag Footer;

} Directory;

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


extern diskAddressPacket kernelSectorBufferInformation;
extern Directory currentDirectory;

void readFolder(unsigned long long LBA);

#endif // ifdef DISK_H
