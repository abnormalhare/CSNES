#ifndef MAPPER_H
#define MAPPER_H

#include "nes.h"

void NROMSetup(NES* this, uint8_t* bytes);
void determineMapper(NES* this, uint8_t* bytes);

#endif