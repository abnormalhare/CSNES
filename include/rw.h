#ifndef RW_H
#define RW_H

#include <stdint.h>
#include "cycle.h"
#include "nes.h"

void write(NES* this, uint16_t addr, uint8_t byte);
uint8_t read(NES* this, uint16_t addr);
void readVal(NES* this);
uint8_t getVal(NES* this);
void ntrnlop(NES* this);

void setupPC(NES* this);

void push(NES* this, uint8_t byte);
uint8_t pop(NES* this, uint16_t* addr);
uint8_t index_z(NES* this, uint8_t byte, uint16_t* addr);
uint8_t index_zx(NES* this, uint8_t byte, uint16_t* addr);
uint8_t index_zy(NES* this, uint8_t byte, uint16_t* addr);
uint8_t index_ax(NES* this, uint16_t byte, uint16_t* addr);
uint8_t index_ay(NES* this, uint16_t byte, uint16_t* addr);
uint8_t index_dx(NES* this, uint8_t byte, uint16_t* addr);
uint8_t index_dy(NES* this, uint8_t byte, uint16_t* addr);

uint16_t index_axw(NES* this, uint16_t byte);
uint16_t index_ayw(NES* this, uint16_t byte);
uint16_t index_dxw(NES* this, uint8_t byte);
uint16_t index_dyw(NES* this, uint8_t byte);
uint8_t index_zxw(NES* this, uint8_t byte);

#endif