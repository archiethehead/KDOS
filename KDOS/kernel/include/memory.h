#ifndef MEMORY_H
#define MEMORY_H

#include "int.h"

int strlen(const char* string);
char strequal(const char* stringOne, const char* stringTwo);
int strcpy(char* dest, size_t destsz, char* src);
int chrcpy(char* dest, char c, size_t count);
uint8_t intToStr(int64_t N, char *str);
uint8_t uintToStr(uint64_t N, char* str);

#endif // ifndef MEMORY_H
