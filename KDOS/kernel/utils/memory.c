#include "memory.h"
#include "console-io.h"

int strlen(const char* string) {

    const char* startAddress = string;

    while (*string != '\0') {
        string++;
    }
    
    return string - startAddress;

}

char strequal(const char* stringOne, const char* stringTwo) {

    while (*stringOne == *stringTwo) {

        if (*stringOne == '\0')
            return 1;

        stringOne++;
        stringTwo++;

    }

    return 0;

}
