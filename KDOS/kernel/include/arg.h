#ifndef ARG_H
#define ARG_H

#include "bool.h"

extern int optind;
extern int optopt;
extern char *optarg;
extern bool optreset;

int getopt(int nargc, char * const nargv[], const char *ostr) ;

#endif // ifdef ARG_H
