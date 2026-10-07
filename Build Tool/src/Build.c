#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

FILE* fileptr;
const size_t megabyte = 1048576;
const size_t MBRSize = 512;
const size_t kernelTargetSize = 16384;
const char* MBRName = "build/boot.bin";
const char* kernName = "build/kernel.bin";
const char* testFile = "build/test.txt";

#pragma pack(push, 1)	

typedef struct {

	char fileName[8];
	uint32_t diskSector;
	uint32_t fileSizeAndFlags;

} DirectoryEntry;

typedef struct {

	uint32_t sizeAndFlags;

} fileTag;

typedef struct {

	uint32_t nextSectorLBA;
	fileTag tag;

} fileFooter;

typedef struct {

	fileTag tag;
	uint32_t fileSize;

} fileHeader;

typedef struct {

	fileHeader header;
	uint8_t data[512 - (sizeof(fileHeader) + sizeof(fileFooter))];
	fileFooter footer;

} fileChunk;

typedef struct {
	
	char directoryName[8];
	uint16_t fileCount;
	uint32_t currentDir;
	uint32_t parentDir;
	uint32_t nextDir;

} DirectoryMetada;

typedef struct {

	fileTag header;
	DirectoryMetada metadata;
	DirectoryEntry entries[30];
	char padding[2];
	fileTag footer;

} Directory;

#pragma pack(pop)

inline size_t getFileSize(FILE* fileptr) {

	long currentPos = ftell(fileptr);
	fseek(fileptr, 0, SEEK_END);
	size_t fileSize = ftell(fileptr);
	fseek(fileptr, currentPos, SEEK_SET);

	return fileSize;

}

uint32_t findFreeSector() {

	long currentPos = (34 * 512);
	fseek(fileptr, currentPos, SEEK_SET);
	fileTag fileSystemSpace;
	fread_s(&fileSystemSpace, sizeof(fileSystemSpace), sizeof(fileSystemSpace), 1, fileptr);
	uint32_t size;

	while (fileSystemSpace.sizeAndFlags >> 31 != 1) {

		size = fileSystemSpace.sizeAndFlags & ~(1 << 31);
		currentPos += (size * 512);
		fseek(fileptr, currentPos, SEEK_SET);
		fread_s(&fileSystemSpace, sizeof(fileSystemSpace), sizeof(fileSystemSpace), 1, fileptr);

	}

	return ftell(fileptr) / 512;

};

void addFile(uint32_t parent, const char* filepath, const char* name) {

	fclose(fileptr);
	fopen_s(&fileptr, filepath, "rb");

	if (!fileptr)
		exit(EXIT_FAILURE);

	size_t fileSize = getFileSize(fileptr);
	size_t chunksRequired = (size_t)ceil(fileSize / (512 - (sizeof(fileHeader) + sizeof(fileFooter))));

	fileChunk* fileChunks = (fileChunk*)malloc(chunksRequired * sizeof(fileChunk));

	if (!fileChunks)
		exit(EXIT_FAILURE);

	fseek(fileptr, 0, SEEK_SET);

	for (int i = 0; i < chunksRequired; i++) {

		fileChunks[i].header.tag.sizeAndFlags = 0 << 31;
		fileChunks[i].header.tag.sizeAndFlags |= 1;
		fileChunks[i].header.fileSize = (uint32_t)fileSize;
		fileChunks[i].footer.tag = fileChunks[i].header.tag;
		fread_s(fileChunks[i].data, sizeof(fileChunks[i].data), 1, sizeof(fileChunks[i].data), fileptr);

	}

	fclose(fileptr);
	fopen_s(&fileptr, "build/KDOS.img", "rb+");

	if (!fileptr)
		exit(EXIT_FAILURE);

	uint32_t freeSectorLBA = findFreeSector();
	uint32_t* nextFileSectorBuffer = (uint32_t*)malloc(chunksRequired * sizeof(uint32_t));

	if (!nextFileSectorBuffer)
		exit(EXIT_FAILURE);

	for (int i = 0; i < chunksRequired; i++) {

		nextFileSectorBuffer[i] = freeSectorLBA;
		uint32_t sectorSize = 0;

		fseek(fileptr, freeSectorLBA * 512, SEEK_SET);
		fread(&sectorSize, sizeof(uint32_t), 1, fileptr);
		sectorSize &= ~(1 << 31);

		fseek(fileptr, freeSectorLBA * 512, SEEK_SET);
		fwrite(&fileChunks[i], sizeof(fileChunks[i]), 1, fileptr);

		if (sectorSize > 1) {
		
			uint32_t buff = sectorSize;
			sectorSize = 1 << 31;
			buff--;
			sectorSize |= buff;
			fwrite(&sectorSize, sizeof(sectorSize), 1, fileptr);
		
		}

		freeSectorLBA = findFreeSector();

	}

	for (int i = 0; i < chunksRequired - 1; i++) {
	
		fileChunks[i].footer.nextSectorLBA = nextFileSectorBuffer[i + 1];
		fseek(fileptr, nextFileSectorBuffer[i] * 512, SEEK_SET);
		fwrite(&fileChunks[i], sizeof(fileChunks[i]), 1, fileptr);

	}

	fileChunks[chunksRequired - 1].footer.nextSectorLBA = 0;
	fseek(fileptr, nextFileSectorBuffer[chunksRequired - 1] * 512, SEEK_SET);
	fwrite(&fileChunks[chunksRequired - 1], sizeof(fileChunks[chunksRequired - 1]), 1, fileptr);

	Directory buffer;
	DirectoryEntry newFile;

	newFile.diskSector = nextFileSectorBuffer[0];
	strcpy_s(newFile.fileName, sizeof(newFile.fileName), name);
	newFile.fileSizeAndFlags = 0 << 30;
	newFile.fileSizeAndFlags |= fileSize;

	fseek(fileptr, parent * 512, SEEK_SET);
	fread_s(&buffer, sizeof(buffer), sizeof(buffer), 1, fileptr);
	buffer.entries[buffer.metadata.fileCount++] = newFile;
	fseek(fileptr, parent * 512, SEEK_SET);
	fwrite(&buffer, sizeof(buffer), 1, fileptr);

	fclose(fileptr);
	return;

}

void addFolder(uint32_t parent, const char* name) {

	Directory newDir;
	newDir.metadata.parentDir = parent;
	strcpy_s(newDir.metadata.directoryName, sizeof(newDir.metadata.directoryName), name);
	long currentPos = (34 * 512);
	fseek(fileptr, 34 * 512, SEEK_SET);
	fileTag fileSystemSpace;
	fread_s(&fileSystemSpace, sizeof(fileSystemSpace), sizeof(fileSystemSpace), 1, fileptr);
	uint32_t size;

	while (fileSystemSpace.sizeAndFlags >> 31 != 1) {

		size = fileSystemSpace.sizeAndFlags & ~(1 << 31);
		currentPos += (size * 512);
		fseek(fileptr, currentPos, SEEK_SET);
		fread_s(&fileSystemSpace, sizeof(fileSystemSpace), sizeof(fileSystemSpace), 1, fileptr);

	}

	size = fileSystemSpace.sizeAndFlags & ~(1 << 31);

	newDir.header.sizeAndFlags = 0 << 31;
	newDir.header.sizeAndFlags |= 1;
	newDir.footer = newDir.header;
	newDir.metadata.currentDir = ftell(fileptr) / 512;
	newDir.metadata.fileCount = 0;
		
	fseek(fileptr, currentPos, SEEK_SET);

	fwrite(&newDir, sizeof(newDir), 1, fileptr);
	fileSystemSpace.sizeAndFlags = 1 << 31;
	fileSystemSpace.sizeAndFlags |= (size - 1);
	fwrite(&fileSystemSpace, sizeof(fileSystemSpace), 1, fileptr);
	fseek(fileptr, ((size * 512) - 512), SEEK_CUR);
	fwrite(&fileSystemSpace, sizeof(fileSystemSpace), 1, fileptr);


	Directory parentDir;
	DirectoryEntry newFile;
	strcpy_s(newFile.fileName, sizeof(newFile.fileName), name);
	newFile.diskSector = newDir.metadata.currentDir;
	newFile.fileSizeAndFlags = 1 << 30;
	newFile.fileSizeAndFlags |= 512;

	fseek(fileptr, parent * 512, SEEK_SET);
	fread_s(&parentDir, sizeof(newDir), sizeof(newDir), 1, fileptr);
	
	parentDir.entries[parentDir.metadata.fileCount] = newFile;
	parentDir.metadata.fileCount++;
	fseek(fileptr, parent * 512, SEEK_SET);
	fwrite(&parentDir, sizeof(parentDir), 1, fileptr);

}

int main() {

	// Read MBR

	size_t bytesRead;

	fopen_s(&fileptr, MBRName, "rb");
	uint8_t* MBRBuffer = (uint8_t*)malloc(512);

	if (!MBRBuffer || !fileptr)
		return EXIT_FAILURE;

	bytesRead = fread_s(MBRBuffer, 512, 1, 512, fileptr);

	if (bytesRead < MBRSize)
		return EXIT_FAILURE;



	// Read Kernel
	
	fclose(fileptr);
	fopen_s(&fileptr, kernName, "rb");

	if (!fileptr)
		return EXIT_FAILURE;

	size_t kernelSize = getFileSize(fileptr);
	size_t* kernelLeftoverSpace = NULL;

	if (kernelSize < kernelTargetSize) {

		kernelLeftoverSpace = (size_t*)malloc(kernelTargetSize - kernelSize);
		if (kernelLeftoverSpace == NULL)
			return EXIT_FAILURE;
		memset(kernelLeftoverSpace, 0, kernelTargetSize - kernelSize);

	}

	else if (kernelSize > kernelTargetSize)
		return EXIT_FAILURE;

	uint8_t* KernelBuffer = (uint8_t*)malloc(kernelSize);

	if (!KernelBuffer)
		return EXIT_FAILURE;

	bytesRead = fread_s(KernelBuffer, kernelSize, 1, kernelSize, fileptr);

	if (bytesRead < kernelSize)
		return EXIT_FAILURE;

	// Write Bootable disk

	fclose(fileptr);
	fopen_s(&fileptr, "build/KDOS.img", "wb+");

	if (!fileptr)
		return EXIT_FAILURE;

	fwrite(MBRBuffer, 512, 1, fileptr);
	fwrite(KernelBuffer, kernelSize, 1, fileptr);

	if (kernelLeftoverSpace != NULL)
		fwrite(kernelLeftoverSpace, kernelTargetSize - kernelSize, 1,fileptr);

	Directory rootDirectory;
	rootDirectory.header.sizeAndFlags = 0U << 31;
	rootDirectory.header.sizeAndFlags |= 1U;
	rootDirectory.metadata.parentDir = 0x0000;
	rootDirectory.metadata.nextDir = 0x0000;
	rootDirectory.metadata.currentDir = 33;
	rootDirectory.footer = rootDirectory.header;
	memcpy_s(rootDirectory.metadata.directoryName, sizeof(rootDirectory.metadata.directoryName), "$", sizeof("$"));
	rootDirectory.metadata.fileCount = 0;
	rootDirectory.padding[0] = 'a';	
	rootDirectory.padding[1] = 'b';

	fwrite(&rootDirectory, sizeof(rootDirectory), 1, fileptr);

	fileTag filesystemHeader;
	filesystemHeader.sizeAndFlags = 1U << 31;
	filesystemHeader.sizeAndFlags |= (megabyte / 512U);

	uint8_t* freeSpace = (uint8_t*)malloc(megabyte);
	if (!freeSpace)
		return EXIT_FAILURE;

	memset(freeSpace, 1, megabyte);

	fwrite(&filesystemHeader, sizeof(filesystemHeader), 1, fileptr);
	fwrite(freeSpace, megabyte, 1, fileptr);
	fwrite(&filesystemHeader, sizeof(filesystemHeader), 1, fileptr);

	addFolder(33, "bin");
	addFolder(33, "user");
	addFolder(33, "sys");
	addFile(33, testFile, "hl2");

	return EXIT_SUCCESS;	

}