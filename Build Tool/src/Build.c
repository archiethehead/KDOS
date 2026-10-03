#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

FILE* Fileptr;
const size_t Megabyte = 1048576;
const size_t MBRSize = 512;
const size_t KernelTargetSize = 16384;
const char* MBRName = "build/boot.bin";
const char* KernName = "build/kernel.bin";


#pragma pack(push, 1)

typedef enum drivenum {

	flpOne = 0,
	flpTwo = 1,
	hddOne = 2,
	hddTwo = 3

} driveNum;

typedef struct {

	char FileName[8];
	uint32_t DiskSector;
	uint32_t FileSizeAndFlags;

} DirectoryEntry;

typedef struct {

	uint32_t SizeAndFlags;

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

inline size_t GetFileSize(FILE* Fileptr) {

	fseek(Fileptr, 0, SEEK_END);
	size_t FileSize = ftell(Fileptr);
	fseek(Fileptr, 0, SEEK_SET);

	return FileSize;

}

void addFolder(uint32_t parent, const char* name) {

	Directory newDir;
	newDir.Metadata.ParentDir = parent;
	strcpy_s(newDir.Metadata.DirectoryName, sizeof(newDir.Metadata.DirectoryName), name);

	fseek(Fileptr, 34 * 512, SEEK_SET);

	FileTag FileSystemSpace;
	fread_s(&FileSystemSpace, sizeof(FileSystemSpace), sizeof(FileSystemSpace), 1, Fileptr);
	uint32_t size;

	while ((FileSystemSpace.SizeAndFlags & ~(1 << 31)) < 512 || FileSystemSpace.SizeAndFlags >> 31 != 1) {
	
		size = FileSystemSpace.SizeAndFlags & ~(1 << 31);
		fseek(Fileptr, (size * 512) + (sizeof(FileTag) * 2), SEEK_CUR);
		fread_s(&FileSystemSpace, sizeof(FileSystemSpace), sizeof(FileSystemSpace), 1, Fileptr);

	}

	size = FileSystemSpace.SizeAndFlags & ~(1 << 31);

	newDir.Header.SizeAndFlags = 0 << 31;
	newDir.Header.SizeAndFlags |= 1;
	newDir.Footer = newDir.Header;
	newDir.Metadata.CurrentDir = ftell(Fileptr) / 512;
	newDir.Metadata.FileCount = 0;
		
	fseek(Fileptr, 0, SEEK_CUR);

	fwrite(&newDir, sizeof(newDir), 1, Fileptr);
	FileSystemSpace.SizeAndFlags = 1 << 31;
	FileSystemSpace.SizeAndFlags |= (size - 1);
	fwrite(&FileSystemSpace, sizeof(FileSystemSpace), 1, Fileptr);
	fseek(Fileptr, ((size * 512) - 512) - 4, SEEK_CUR);
	fwrite(&FileSystemSpace, sizeof(FileSystemSpace), 1, Fileptr);


	Directory parentDir;
	DirectoryEntry newFile;
	strcpy_s(newFile.FileName, sizeof(newFile.FileName), name);
	newFile.DiskSector = newDir.Metadata.CurrentDir;
	newFile.FileSizeAndFlags = 3 << 30;
	newFile.FileSizeAndFlags |= 512;

	fseek(Fileptr, parent * 512, SEEK_SET);
	fread_s(&parentDir, sizeof(newDir), sizeof(newDir), 1, Fileptr);
	
	parentDir.Entries[parentDir.Metadata.FileCount] = newFile;
	parentDir.Metadata.FileCount++;
	fseek(Fileptr, parent * 512, SEEK_SET);
	fwrite(&parentDir, sizeof(parentDir), 1, Fileptr);

}

int main() {

	// Read MBR

	size_t BytesRead;

	fopen_s(&Fileptr, MBRName, "rb");
	uint8_t* MBRBuffer = (uint8_t*)malloc(512);

	if (!MBRBuffer || !Fileptr)
		return EXIT_FAILURE;

	BytesRead = fread_s(MBRBuffer, 512, 1, 512, Fileptr);

	if (BytesRead < MBRSize)
		return EXIT_FAILURE;



	// Read Kernel

	fopen_s(&Fileptr, KernName, "rb");

	if (!Fileptr)
		return EXIT_FAILURE;

	size_t KernelSize = GetFileSize(Fileptr);
	size_t* KernelLeftoverSpace = NULL;

	if (KernelSize < KernelTargetSize) {

		KernelLeftoverSpace = (size_t*)malloc(KernelTargetSize - KernelSize);
		if (KernelLeftoverSpace == NULL)
			return EXIT_FAILURE;
		memset(KernelLeftoverSpace, 0, KernelTargetSize - KernelSize);

	}

	else if (KernelSize > KernelTargetSize)
		return EXIT_FAILURE;

	uint8_t* KernelBuffer = (uint8_t*)malloc(KernelSize);

	if (!KernelBuffer)
		return EXIT_FAILURE;

	BytesRead = fread_s(KernelBuffer, KernelSize, 1, KernelSize, Fileptr);

	if (BytesRead < KernelSize)
		return EXIT_FAILURE;

	// Write Bootable disk

	fopen_s(&Fileptr, "build/KDOS.img", "wb+");

	if (!Fileptr)
		return EXIT_FAILURE;

	fwrite(MBRBuffer, 512, 1, Fileptr);
	fwrite(KernelBuffer, KernelSize, 1, Fileptr);

	if (KernelLeftoverSpace != NULL)
		fwrite(KernelLeftoverSpace, KernelTargetSize - KernelSize, 1,Fileptr);

	Directory RootDirectory;
	RootDirectory.Header.SizeAndFlags = 0U << 31;
	RootDirectory.Header.SizeAndFlags |= 1U;
	RootDirectory.Metadata.ParentDir = 0x0000;
	RootDirectory.Metadata.NextDir = 0x0000;
	RootDirectory.Metadata.CurrentDir = 33;
	RootDirectory.Footer = RootDirectory.Header;
	memcpy_s(RootDirectory.Metadata.DirectoryName, sizeof(RootDirectory.Metadata.DirectoryName), "$", sizeof("$"));
	RootDirectory.Metadata.FileCount = 0;
	RootDirectory.padding[0] = 'a';	
	RootDirectory.padding[1] = 'b';

	fwrite(&RootDirectory, sizeof(RootDirectory), 1, Fileptr);

	FileTag FilesystemHeader;
	FilesystemHeader.SizeAndFlags = 1U << 31;
	FilesystemHeader.SizeAndFlags |= (Megabyte / 512U);

	uint8_t* FreeSpace = (uint8_t*)malloc(Megabyte);
	if (!FreeSpace)
		return EXIT_FAILURE;

	memset(FreeSpace, 1, Megabyte);

	fwrite(&FilesystemHeader, sizeof(FilesystemHeader), 1, Fileptr);
	fwrite(FreeSpace, Megabyte, 1, Fileptr);
	fwrite(&FilesystemHeader, sizeof(FilesystemHeader), 1, Fileptr);

	addFolder(33, "binary");

	return EXIT_SUCCESS;	

}