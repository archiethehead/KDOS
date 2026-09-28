#ifndef MEMORY_H
#define MEMORY_H

#include "int.h"

int strlen(const char* string);
char strequal(const char* stringOne, const char* stringTwo);
int strcpy(char* dest, size_t destsz, char* src);
int chrcpy(char* dest, char c, size_t count);
void intToStr(int N, char *str);
void ulongToStr(uint32_t N, char *str);

#endif // ifndef MEMORY_H
