#ifndef MEMORY_H
#define MEMORY_H

int strlen(const char* string);
char strequal(const char* stringOne, const char* stringTwo);
int strcpy(char* dest, unsigned long long destsz, char* src);
int chrcpy(char* dest, char c, unsigned long long count);
void intToStr(int N, char *str);
void ulongToStr(unsigned long N, char *str);

#endif // ifndef MEMORY_H
