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

	char Name[8];
	uint32_t DiskSector;
	uint32_t FileSize;

} DirectoryEntry;

typedef struct {

	uint32_t Size;

} FileTag;

typedef struct {

	uint16_t FileCount;
	uint32_t CurrentDir;
	uint32_t ParentDir;
	uint32_t NextDir;
	char padding[2];

} DirectoryMetada;

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



	return EXIT_SUCCESS;	

}