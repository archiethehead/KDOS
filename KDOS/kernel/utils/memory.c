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


// GeeksforGeeks (2024). 
// How to Convert an Integer to a String in C? 
// [online] GeeksforGeeks. 
// Available at: https://www.geeksforgeeks.org/c/how-to-convert-an-integer-to-a-string-in-c/ 
// [Accessed 22 Sept. 2026].

void intToStr(int N, char *str) {
    
    int i = 0;
    int sign = N;
    if (N < 0)
        N = -N;

    while (N > 0) {
      
        str[i++] = N % 10 + '0';
      	N /= 10;
    } 

    if (sign < 0) {

        str[i++] = '-';
        
    }
    str[i] = '\0';

    for (int j = 0, k = i - 1; j < k; j++, k--) {

        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;

    }
    
}

void ulongToStr(unsigned long N, char *str) {
    
    int i = 0;
    while (N > 0) {
      
        str[i++] = N % 10 + '0';
      	N /= 10;
    } 

    str[i] = '\0';

    for (int j = 0, k = i - 1; j < k; j++, k--) {

        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;

    }
    
}