#include "cycle.h"
#include "nes.h"

int main(int argc, char* argv[]) {
    NES* nes;
    uint32_t delay;
    int64_t lastTime;
    FILE* file;
    size_t filesize;

    if (argc != 2) {
        printf("Format: %s <ROM>\n", argv[0]);
        return 1;
    }

    char const* filename = argv[1];

    file = fopen(filename, "rb");
    if (file == NULL) {
        printf("ERROR: Unable to open '%s'.", filename);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    nes = newNES();
    NESLoadROM(nes, file, filename, filesize);

    setupPC(nes);

    lastTime = timeInCycles();
    while (1) {
        delay = nes->cycleFunc(nes);
        if (delay == -1) break;
        while (timeInCycles() - lastTime < delay);
        lastTime = timeInCycles();
    }

    return 0;
}