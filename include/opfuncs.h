#ifndef OPFUNCS_H
#define OPFUNCS_H

#include "nes.h"

extern uint32_t page_crossed;

void push(NES* this, uint8_t byte);
uint8_t pop(NES* this);
uint8_t index_zx(NES* this, uint8_t byte);
uint8_t index_zy(NES* this, uint8_t byte);
uint8_t index_ax(NES* this, uint16_t addr);
uint8_t index_ay(NES* this, uint16_t addr);
uint8_t index_dx(NES* this, uint8_t ind);
uint8_t index_dy(NES* this, uint8_t ind);

void OP_ADC(NES* this, uint16_t mem);
void OP_AND(NES* this, uint8_t mem);
void OP_ASL(NES* this, uint8_t* mem);
void OP_CMP(NES* this, uint8_t mem);
void OP_CPX(NES* this, uint8_t mem);
void OP_CPY(NES* this, uint8_t mem);
void OP_DEC(NES* this, uint8_t* mem);
void OP_INC(NES* this, uint8_t* mem);
void OP_EOR(NES* this, uint8_t mem);
void OP_LDA(NES* this, uint8_t mem);
void OP_LDX(NES* this, uint8_t mem);
void OP_LDY(NES* this, uint8_t mem);
void OP_LSR(NES* this, uint8_t* mem);
void OP_ORA(NES* this, uint8_t mem);
void OP_ROL(NES* this, uint8_t* mem);
void OP_ROR(NES* this, uint8_t* mem);
void OP_SBC(NES* this, uint8_t mem);

#endif