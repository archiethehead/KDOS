#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

FILE* Fileptr_s;
const char* SMBRName = "build/boot.bin";
const char* SKernName = "build/kernel.bin";

int main() {

	size_t STBytesRead;

	fopen_s(&Fileptr_s, SMBRName, "r");
	uint8_t* UBMBRBuffer = (uint8_t*)malloc(512);

	if (!UBMBRBuffer || !Fileptr_s)
		return EXIT_FAILURE;

	STBytesRead = fread_s(UBMBRBuffer, 512, 1, 512, Fileptr_s);

	fopen_s(&Fileptr_s, SKernName, "r");

	if (!Fileptr_s)
		return EXIT_FAILURE;

	fseek(Fileptr_s, 0, SEEK_END);
	size_t Size = ftell(Fileptr_s);
	fseek(Fileptr_s, 0, SEEK_SET);

	uint8_t* UBKernBuffer = (uint8_t*)malloc(Size);

	if (!UBKernBuffer)
		return EXIT_FAILURE;

	STBytesRead = fread_s(UBKernBuffer, Size, 1, Size, Fileptr_s);

	fopen_s(&Fileptr_s, "build/KDOS.img", "w");

	if (!Fileptr_s)
		return EXIT_FAILURE;

	fwrite(UBMBRBuffer, 512, 1, Fileptr_s);
	fwrite(UBKernBuffer, Size, 1, Fileptr_s);

}