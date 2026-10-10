#include "time.h"
#include "console-io.h"

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

uint32_t getTime() {

    uint16_t systemTicksHigh = 0;
    uint16_t systemTicksLow = 0;

    __asm {

        mov ah, 0x00
        int 0x1A
        mov systemTicksHigh, cx
        mov systemTicksLow, dx

    }

    uint32_t systemTicks = ((uint32_t)systemTicksHigh << 16) | systemTicksLow;

    return systemTicks;

}
