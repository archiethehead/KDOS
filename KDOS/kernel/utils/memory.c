#include "memory.h"
#include "console-io.h"
#include "bool.h"

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

int strcpy(char* dest, size_t destsz, char* src) {

    if (!destsz || !(*src))
        return 0;

    size_t charsCopied = 0;

    while (src[charsCopied] != '\0' && charsCopied < (destsz - 1)) {

        dest[charsCopied] = src[charsCopied];
        charsCopied++;

    }
    
    dest[charsCopied] = '\0';
    return charsCopied;

}

int chrcpy(char* dest, char c, size_t count) {

    if (!count)
        return 0;

    size_t charsCopied = 0;

    while (count--) {

        *dest = c;
        dest++;
        charsCopied++;

    }

    return charsCopied;
    
}

// GeeksforGeeks (2024). 
// How to Convert an Integer to a String in C? 
// [online] GeeksforGeeks. 
// Available at: https://www.geeksforgeeks.org/c/how-to-convert-an-integer-to-a-string-in-c/ 
// [Accessed 22 Sept. 2026].

static void numToStr(int64_t N, char *str, bool isNegative) {
    
    // base case
    if (N == 0) {

        str[0] = '0';
        str[1] = '\0';
        return;

    }

    uint16_t NWords[4];
    NWords[0] = (uint16_t)N;
    NWords[1] = (uint16_t)(N >> 16);
    NWords[2] = (uint16_t)(N >> 32);
    NWords[3] = (uint16_t)(N >> 48);

    uint8_t highestWord = 3;
    while (highestWord > 0 && NWords[highestWord] == 0)
        highestWord--;

    uint8_t i = 0;

    while (true) {

        uint32_t buffer32 = 0;
        uint16_t remainder = 0;
        bool isZero = true;

        for (int8_t j = highestWord; j >= 0; j--) {

            buffer32 = ((uint32_t)remainder << 16) | NWords[j];
            NWords[j] = (uint16_t)(buffer32 / 10);
            remainder = (uint16_t)(buffer32 % 10);

            if (NWords[j] > 0)
                isZero = false;

        }

        str[i++] = remainder + '0';

        if (isZero)
            break;

        while (highestWord > 0 && NWords[highestWord] == 0)
            highestWord--;

    } 

    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';

    for (uint8_t j = 0, k = i - 1; j < k; j++, k--) {

        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;

    }
    
}

void intToStr(int64_t N, char* str) {

    bool isNegative = false;
    uint64_t absoluteValue = (uint64_t)N;

    if (N < 0) {

        isNegative = true;
        absoluteValue = -(uint64_t)N;

    }

    numToStr(absoluteValue, str, isNegative);

};

void uintToStr(uint64_t N, char* str) {

    numToStr(N, str, false);

}
