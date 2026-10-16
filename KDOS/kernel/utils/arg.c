#include "arg.h"
#include "memory.h"
#include "console-io.h"

int optind = 0;
int optopt = 1;
char *optarg;
bool optreset = false;

#define BADCH   (int)'?'
#define BADARG  (int)':'
#define EMSG    ""

int getopt(int nargc, char * const nargv[], const char *ostr)  {  

    static char *place = EMSG;
    const char *oli;

    if (optreset || !*place) {

        if (optreset)
            optind = 0;

        optreset = 0;
        if (optind >= nargc || *(place = nargv[optind]) != '-') {

            place = EMSG;
            return (-1);

        }

        if (place[1] && *++place == '-') {

            ++optind;
            place = EMSG;
            return (-1);

        }

    }

    if ((optopt = (int)*place++) == (int)':' || !(oli = strchr(ostr, (char)optopt))) {

        if (optopt == (int)'-')
            return (-1);

        if (!*place)
            ++optind;

        return (BADCH);

    }

    if (*++oli != ':') {

        optarg = 0;
        if (!*place)
        ++optind;

    }

    else {

        if (*place)
            optarg = place;

        else if (nargc <= ++optind) {
            
            place = EMSG;

            if (*ostr == ':')
                return (BADARG);

            return (BADCH);

        }

        else
            optarg = nargv[optind];

        place = EMSG;
        ++optind;

    }

    return (optopt);

}
