#include "video.h"
#include "math.h"

uint8_t far *VGAMemory = SOA(0xA000, 0x0000);
videoMode currentVideoMode = text;

void setVideoMode(videoMode mode) {

    __asm {

        mov ah, 0x00
        mov al, mode
        int 0x10

    }

    currentVideoMode = mode;

}

bool drawXBM(uint16_t width, uint16_t height, uint8_t* imageData) {

    if (currentVideoMode != VGA)
        return false;

    uint8_t byte = 0;
    uint16_t pixelIndex = 0;
   
    for (int y = 0; y < height; y++) {

        for (int x = 0; x < width; x++) {

            if (pixelIndex % 8 == 0)
                byte = imageData[pixelIndex / 8];
            
            bool isBlack = ((byte & (1 << (pixelIndex % 8))) >> (pixelIndex % 8));
            uint16_t offset = (y * 320) + x;
            VGAMemory[offset] = isBlack ? 0 : 15;
            pixelIndex++;

        }

    }

    return true;

}
