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

    uint16_t widthOffset = (320 - width) / 2;
    uint16_t heightOffset = (200 - height) / 2;

    if (currentVideoMode != VGA)
        return false;

    uint8_t byte = 0;
    uint16_t pixelIndex = 0;
   
    for (int y = 0; y < height; y++) {

        for (int x = 0; x < width; x++) {

            if (pixelIndex % 8 == 0)
                byte = imageData[pixelIndex / 8];
            
            bool isBlack = EXTRACT_BIT(byte, pixelIndex % 8);
            uint16_t offset = ((y + heightOffset) * 320) + (x + widthOffset);
            VGAMemory[offset] = isBlack ? 0 : 15;
            pixelIndex++;

        }

    }

    return true;

}
