#ifndef MATH_H
#define MATH_H

#include "int.h"

#define SOA(x, y) (uint8_t far *)(((uint32_t)x << 16) | y)
#define EXTRACT_BIT(x, y) ((x & (1 << y)) >> y);

typedef struct {

    uint8_t x;
    uint8_t y;

} vector2;

#endif // ifdef MATH_H
