#include "ppu.h"

void writeToScreen(NES* this, Dot val, int scanline, int dot) {
    this->ppu.screen[scanline * 341 + dot] = val;
}

void PPUCycle(NES* this) {
    switch (this->ppu.scanline) {
        case 261:
            // ...
        default:
            break;
    }
    this->ppu.dot++;
    if (this->ppu.dot == 341) {
        this->ppu.dot = 0;
        this->ppu.scanline++;
        if (this->ppu.scanline == 262) {
            this->ppu.scanline = 0;
        }
    }
}