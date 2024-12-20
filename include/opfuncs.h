#ifndef OPFUNCS_H
#define OPFUNCS_H

#include <time.h>
#include "nes.h"

void OP_ADC(NES* this, uint16_t mem);
void OP_AND(NES* this, uint8_t mem);
void OP_ASL(NES* this, uint8_t* mem);
void OP_CMP(NES* this, uint8_t mem);
void OP_CPX(NES* this, uint8_t mem);
void OP_CPY(NES* this, uint8_t mem);
void OP_DCP(NES* this, uint8_t* mem);
void OP_DEC(NES* this, uint8_t* mem);
void OP_EOR(NES* this, uint8_t mem);
void OP_INC(NES* this, uint8_t* mem);
void OP_ISC(NES* this, uint8_t* mem);
void OP_LAX(NES* this, uint8_t mem);
void OP_LDA(NES* this, uint8_t mem);
void OP_LDX(NES* this, uint8_t mem);
void OP_LDY(NES* this, uint8_t mem);
void OP_LSR(NES* this, uint8_t* mem);
void OP_ORA(NES* this, uint8_t mem);
void OP_ROL(NES* this, uint8_t* mem);
void OP_ROR(NES* this, uint8_t* mem);
void OP_RLA(NES* this, uint8_t* mem);
void OP_RRA(NES* this, uint8_t* mem);
void OP_SBC(NES* this, uint8_t mem);
void OP_SLO(NES* this, uint8_t* mem);
void OP_SRE(NES* this, uint8_t* mem);

#endif