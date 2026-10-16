#ifndef PROGRAM_H
#define PROGRAM_H

typedef struct {

    int (*main)(int, char**) = 0;
    void (*printString)(const char*) = 0;

} programHeader;

#endif // ifdef PROGRAM_H