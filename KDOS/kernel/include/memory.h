#ifndef MEMORY_H
#define MEMORY_H

#include "int.h"
#include "bool.h"

uint16_t strlen(const char* string);
bool strequal(const char* stringOne, const char* stringTwo);
uint16_t strcpy(char* dest, uint16_t destsz, char* src);
uint16_t chrcpy(char* dest, char c, uint16_t count);
uint8_t intToStr(int64_t N, char *str);
uint8_t uintToStr(uint64_t N, char* str);
uint16_t memcopy(void* dest, uint16_t destsz, uint16_t count, void* src);

#endif // ifndef MEMORY_H
