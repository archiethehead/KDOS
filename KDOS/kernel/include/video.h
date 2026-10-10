#ifndef VIDEO_H
#define VIDEO_H

#include "int.h"
#include "bool.h"

typedef enum {

    VGA     = 0x13,
    text    = 0x03

} videoMode;

void setVideoMode(videoMode mode);
bool drawXBM(uint16_t width, uint16_t height, uint8_t* imageData);

#endif // ifdef VIDEO_H
