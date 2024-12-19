#include "cycle.h"

int64_t timeInNanoseconds(void) {
    struct timeval tv;

    mingw_gettimeofday(&tv,NULL);
    return ((int64_t)tv.tv_sec) * 1E9 + tv.tv_usec;
}

double timeInCycles(void) {
    int64_t tv = timeInNanoseconds();

    return tv / 559.0;
}