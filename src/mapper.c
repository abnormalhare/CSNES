#include "mapper.h"

void NROMSetup(NES* this, uint8_t* bytes) {
    int rAddr = 0;
    int fAddr = 0;
    int i;

    if (this->header.has_trainer) {
        for (i = 0x7000; i < 0x7200; i++, fAddr++) {
            this->SRAM[i + 0x1000] = bytes[fAddr];
        }
    }

    for (i = 0; i < this->header.prgrom_size * 0x4000; i++, fAddr++) {
        prgROM[i] = bytes[fAddr];
    }

    this->PRGROM = prgROM; // set the pointer to prgROM (bank switching, essentially)

    for (i = 0; i < this->header.chrrom_size * 0x2000; i++, fAddr++) {
        chrROM[i] = bytes[fAddr];
    }

    this->ppu.patternTbls = chrROM;
}

void determineMapper(NES* this, uint8_t* bytes) {
    uint16_t mapper = this->header.mapper_lo + (this->header.mapper_hi << 4) + (this->header.mapper_hiest << 8);

    switch (mapper)
    {
    case 0:
        NROMSetup(this, bytes);
        break;
    
    default:
        printf("ERROR: unimplemented mapper\n");
        exit(1);
    }
}