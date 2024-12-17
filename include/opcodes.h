#ifndef OPCODES_H
#define OPCODES_H

#include "nes.h"

#define VAL1 this->RAM[this->pc + 1]
#define VAL2 this->RAM[this->pc + 2]
#define CHECK_PAGE(f)     \
    if (page_crossed) {   \
        return f + 1;     \
    } else {              \
        return f;         \
    }

uint32_t OP_STP(NES* this);
uint32_t OP_00(NES* this);
uint32_t OP_01(NES* this);
uint32_t OP_02(NES* this);
uint32_t OP_04(NES* this);
uint32_t OP_05(NES* this);
uint32_t OP_06(NES* this);
uint32_t OP_07(NES* this);
uint32_t OP_08(NES* this);
uint32_t OP_09(NES* this);
uint32_t OP_0A(NES* this);
uint32_t OP_0B(NES* this);
uint32_t OP_0C(NES* this);
uint32_t OP_0D(NES* this);
uint32_t OP_0E(NES* this);
uint32_t OP_0F(NES* this);
uint32_t OP_10(NES* this);
uint32_t OP_11(NES* this);
uint32_t OP_12(NES* this);
uint32_t OP_14(NES* this);
uint32_t OP_15(NES* this);
uint32_t OP_16(NES* this);
uint32_t OP_17(NES* this);
uint32_t OP_18(NES* this);
uint32_t OP_19(NES* this);
uint32_t OP_1A(NES* this);
uint32_t OP_1B(NES* this);
uint32_t OP_1C(NES* this);
uint32_t OP_1D(NES* this);
uint32_t OP_1E(NES* this);
uint32_t OP_1F(NES* this);
uint32_t OP_20(NES* this);
uint32_t OP_21(NES* this);
uint32_t OP_22(NES* this);
uint32_t OP_24(NES* this);
uint32_t OP_25(NES* this);
uint32_t OP_26(NES* this);
uint32_t OP_27(NES* this);
uint32_t OP_28(NES* this);
uint32_t OP_29(NES* this);
uint32_t OP_2A(NES* this);
uint32_t OP_2B(NES* this);
uint32_t OP_2C(NES* this);
uint32_t OP_2D(NES* this);
uint32_t OP_2E(NES* this);
uint32_t OP_2F(NES* this);
uint32_t OP_30(NES* this);
uint32_t OP_31(NES* this);
uint32_t OP_32(NES* this);
uint32_t OP_34(NES* this);
uint32_t OP_35(NES* this);
uint32_t OP_36(NES* this);
uint32_t OP_37(NES* this);
uint32_t OP_38(NES* this);
uint32_t OP_39(NES* this);
uint32_t OP_3A(NES* this);
uint32_t OP_3B(NES* this);
uint32_t OP_3C(NES* this);
uint32_t OP_3D(NES* this);
uint32_t OP_3E(NES* this);
uint32_t OP_3F(NES* this);

#endif