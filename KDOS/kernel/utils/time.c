#include "time.h"

void wait(uint16_t seconds) {

    uint32_t microseconds = seconds * 1000000;
    uint16_t highWord = microseconds >> 16;
    uint16_t lowWord = (uint16_t)microseconds;

    __asm {

        mov ah, 0x86
        mov cx, highWord
        mov dx, lowWord
        int 0x15

    }

}
