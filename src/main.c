#include <time.h>
#include "nes.h"

int64_t timeInMilliseconds(void) {
    struct timeval tv;

    mingw_gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

int main(int argc, char* argv[]) {
    NES* nes;
    uint32_t delay;
    int64_t lastTime;

    if (argc != 2) {
        printf("Format: %s <ROM>\n", argv[0]);
        return 1;
    }

    char const* filename = argv[1];

    nes = newNES();
    NESLoadROM(nes, filename);

    lastTime = timeInMilliseconds();
    while (1) {
        delay += nes->cycleFunc(nes);
        if (delay < 29,830) {
            while (timeInMilliseconds() - lastTime < 16667);
        }
    }

    return 0;
}