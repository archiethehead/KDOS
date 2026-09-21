#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

FILE* Fileptr;
const size_t MBRSize = 512;
const char* MBRName = "build/boot.bin";
const char* KernName = "build/kernel.bin";


#pragma pack(push, 1)

typedef struct {

	char FileName[8];
	uint32_t DiskSector;
	uint32_t FileSizeAndFlags;

} DirectoryEntry;

typedef struct {

	uint32_t Size;

} FileTag;

typedef struct {
	
	char DirectoryName[8];
	uint16_t FileCount;
	uint32_t CurrentDir;
	uint32_t ParentDir;
	uint32_t NextDir;

} DirectoryMetada;

typedef struct {

	FileTag Header;
	DirectoryMetada Metadata;
	DirectoryEntry Entries[30];
	char padding[2];
	FileTag Footer;

} Directory;

#pragma pack(pop)

int main() {

	// Read MBR

	size_t BytesRead;

	fopen_s(&Fileptr, MBRName, "r");
	uint8_t* MBRBuffer = (uint8_t*)malloc(512);

	if (!MBRBuffer || !Fileptr)
		return EXIT_FAILURE;

	BytesRead = fread_s(MBRBuffer, 512, 1, 512, Fileptr);

	if (BytesRead < MBRSize)
		return EXIT_FAILURE;



	// Read Kernel

	fopen_s(&Fileptr, KernName, "r");

	if (!Fileptr)
		return EXIT_FAILURE;

	fseek(Fileptr, 0, SEEK_END);
	size_t KernelSize = ftell(Fileptr);
	fseek(Fileptr, 0, SEEK_SET);

	uint8_t* KernelBuffer = (uint8_t*)malloc(KernelSize);

	if (!KernelBuffer)
		return EXIT_FAILURE;

	BytesRead = fread_s(KernelBuffer, KernelSize, 1, KernelSize, Fileptr);

	if (BytesRead < KernelSize)
		return EXIT_FAILURE;



	// Write Bootable disk

	fopen_s(&Fileptr, "build/KDOS.img", "w");

	if (!Fileptr)
		return EXIT_FAILURE;

	fwrite(MBRBuffer, 512, 1, Fileptr);
	fwrite(KernelBuffer, KernelSize, 1, Fileptr);

	Directory RootDirectory;
	RootDirectory.Header.Size = 3 << 30;
	RootDirectory.Header.Size |= 1;
	RootDirectory.Footer = RootDirectory.Header;
	memcpy_s(RootDirectory.Metadata.DirectoryName, sizeof(RootDirectory.Metadata.DirectoryName), "Root", sizeof("Root"));
	RootDirectory.Metadata.FileCount = 0;
	RootDirectory.padding[0] = 'a';
	RootDirectory.padding[1] = 'b';

	fwrite(&RootDirectory, sizeof(RootDirectory), 1, Fileptr);

	return EXIT_SUCCESS;	

}