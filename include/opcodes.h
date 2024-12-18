#ifndef OPCODES_H
#define OPCODES_H

#include "nes.h"
#include "opfuncs.h"

#define VAL1 *read(this, this->pc + 1)
#define VAL2 *read(this, this->pc + 2)
#define LE_ADDR *read(this, this->pc + 1) + (*read(this, this->pc + 2) << 8)
#define CHECK_PAGE(f)     \
    if (page_crossed) {   \
        return f + 1;     \
    } else {              \
        return f;         \
    }

uint32_t OP_STP(NES* this);
uint32_t OP_00(NES* this); uint32_t OP_01(NES* this); uint32_t OP_03(NES* this); uint32_t OP_04(NES* this);
uint32_t OP_05(NES* this); uint32_t OP_06(NES* this); uint32_t OP_07(NES* this); uint32_t OP_08(NES* this);
uint32_t OP_09(NES* this); uint32_t OP_0A(NES* this); uint32_t OP_0B(NES* this); uint32_t OP_0C(NES* this);
uint32_t OP_0D(NES* this); uint32_t OP_0E(NES* this); uint32_t OP_0F(NES* this);

uint32_t OP_10(NES* this); uint32_t OP_11(NES* this); uint32_t OP_13(NES* this); uint32_t OP_14(NES* this);
uint32_t OP_15(NES* this); uint32_t OP_16(NES* this); uint32_t OP_17(NES* this); uint32_t OP_18(NES* this);
uint32_t OP_19(NES* this); uint32_t OP_1A(NES* this); uint32_t OP_1B(NES* this); uint32_t OP_1C(NES* this);
uint32_t OP_1D(NES* this); uint32_t OP_1E(NES* this); uint32_t OP_1F(NES* this);

uint32_t OP_20(NES* this); uint32_t OP_21(NES* this); uint32_t OP_23(NES* this); uint32_t OP_24(NES* this);
uint32_t OP_25(NES* this); uint32_t OP_26(NES* this); uint32_t OP_27(NES* this); uint32_t OP_28(NES* this);
uint32_t OP_29(NES* this); uint32_t OP_2A(NES* this); uint32_t OP_2B(NES* this); uint32_t OP_2C(NES* this);
uint32_t OP_2D(NES* this); uint32_t OP_2E(NES* this); uint32_t OP_2F(NES* this);

uint32_t OP_30(NES* this); uint32_t OP_31(NES* this); uint32_t OP_33(NES* this); uint32_t OP_34(NES* this);
uint32_t OP_35(NES* this); uint32_t OP_36(NES* this); uint32_t OP_37(NES* this); uint32_t OP_38(NES* this);
uint32_t OP_39(NES* this); uint32_t OP_3A(NES* this); uint32_t OP_3B(NES* this); uint32_t OP_3C(NES* this);
uint32_t OP_3D(NES* this); uint32_t OP_3E(NES* this); uint32_t OP_3F(NES* this);

uint32_t OP_40(NES* this); uint32_t OP_41(NES* this); uint32_t OP_43(NES* this); uint32_t OP_44(NES* this);
uint32_t OP_45(NES* this); uint32_t OP_46(NES* this); uint32_t OP_47(NES* this); uint32_t OP_48(NES* this);
uint32_t OP_49(NES* this); uint32_t OP_4A(NES* this); uint32_t OP_4B(NES* this); uint32_t OP_4C(NES* this);
uint32_t OP_4D(NES* this); uint32_t OP_4E(NES* this); uint32_t OP_4F(NES* this);

uint32_t OP_50(NES* this); uint32_t OP_51(NES* this); uint32_t OP_53(NES* this); uint32_t OP_54(NES* this);
uint32_t OP_55(NES* this); uint32_t OP_56(NES* this); uint32_t OP_57(NES* this); uint32_t OP_58(NES* this);
uint32_t OP_59(NES* this); uint32_t OP_5A(NES* this); uint32_t OP_5B(NES* this); uint32_t OP_5C(NES* this);
uint32_t OP_5D(NES* this); uint32_t OP_5E(NES* this); uint32_t OP_5F(NES* this);

uint32_t OP_60(NES* this); uint32_t OP_61(NES* this); uint32_t OP_63(NES* this); uint32_t OP_64(NES* this);
uint32_t OP_65(NES* this); uint32_t OP_66(NES* this); uint32_t OP_67(NES* this); uint32_t OP_68(NES* this);
uint32_t OP_69(NES* this); uint32_t OP_6A(NES* this); uint32_t OP_6B(NES* this); uint32_t OP_6C(NES* this);
uint32_t OP_6D(NES* this); uint32_t OP_6E(NES* this); uint32_t OP_6F(NES* this);

uint32_t OP_70(NES* this); uint32_t OP_71(NES* this); uint32_t OP_73(NES* this); uint32_t OP_74(NES* this);
uint32_t OP_75(NES* this); uint32_t OP_76(NES* this); uint32_t OP_77(NES* this); uint32_t OP_78(NES* this);
uint32_t OP_79(NES* this); uint32_t OP_7A(NES* this); uint32_t OP_7B(NES* this); uint32_t OP_7C(NES* this);
uint32_t OP_7D(NES* this); uint32_t OP_7E(NES* this); uint32_t OP_7F(NES* this);

uint32_t OP_80(NES* this); uint32_t OP_81(NES* this); uint32_t OP_82(NES* this); uint32_t OP_83(NES* this);
uint32_t OP_84(NES* this); uint32_t OP_85(NES* this); uint32_t OP_86(NES* this); uint32_t OP_87(NES* this);
uint32_t OP_88(NES* this); uint32_t OP_89(NES* this); uint32_t OP_8A(NES* this); uint32_t OP_8B(NES* this);
uint32_t OP_8C(NES* this); uint32_t OP_8D(NES* this); uint32_t OP_8E(NES* this); uint32_t OP_8F(NES* this);

uint32_t OP_90(NES* this); uint32_t OP_91(NES* this); uint32_t OP_93(NES* this); uint32_t OP_94(NES* this);
uint32_t OP_95(NES* this); uint32_t OP_96(NES* this); uint32_t OP_97(NES* this); uint32_t OP_98(NES* this);
uint32_t OP_99(NES* this); uint32_t OP_9A(NES* this); uint32_t OP_9B(NES* this); uint32_t OP_9C(NES* this);
uint32_t OP_9D(NES* this); uint32_t OP_9E(NES* this); uint32_t OP_9F(NES* this);

uint32_t OP_A0(NES* this); uint32_t OP_A1(NES* this); uint32_t OP_A2(NES* this); uint32_t OP_A3(NES* this);
uint32_t OP_A4(NES* this); uint32_t OP_A5(NES* this); uint32_t OP_A6(NES* this); uint32_t OP_A7(NES* this);
uint32_t OP_A8(NES* this); uint32_t OP_A9(NES* this); uint32_t OP_AA(NES* this); uint32_t OP_AB(NES* this);
uint32_t OP_AC(NES* this); uint32_t OP_AD(NES* this); uint32_t OP_AE(NES* this); uint32_t OP_AF(NES* this);

uint32_t OP_B0(NES* this); uint32_t OP_B1(NES* this); uint32_t OP_B3(NES* this); uint32_t OP_B4(NES* this);
uint32_t OP_B5(NES* this); uint32_t OP_B6(NES* this); uint32_t OP_B7(NES* this); uint32_t OP_B8(NES* this);
uint32_t OP_B9(NES* this); uint32_t OP_BA(NES* this); uint32_t OP_BB(NES* this); uint32_t OP_BC(NES* this);
uint32_t OP_BD(NES* this); uint32_t OP_BE(NES* this); uint32_t OP_BF(NES* this);

uint32_t OP_C0(NES* this); uint32_t OP_C1(NES* this); uint32_t OP_C2(NES* this); uint32_t OP_C3(NES* this);
uint32_t OP_C4(NES* this); uint32_t OP_C5(NES* this); uint32_t OP_C6(NES* this); uint32_t OP_C7(NES* this);
uint32_t OP_C8(NES* this); uint32_t OP_C9(NES* this); uint32_t OP_CA(NES* this); uint32_t OP_CB(NES* this);
uint32_t OP_CC(NES* this); uint32_t OP_CD(NES* this); uint32_t OP_CE(NES* this); uint32_t OP_CF(NES* this);

uint32_t OP_D0(NES* this); uint32_t OP_D1(NES* this); uint32_t OP_D3(NES* this); uint32_t OP_D4(NES* this);
uint32_t OP_D5(NES* this); uint32_t OP_D6(NES* this); uint32_t OP_D7(NES* this); uint32_t OP_D8(NES* this);
uint32_t OP_D9(NES* this); uint32_t OP_DA(NES* this); uint32_t OP_DB(NES* this); uint32_t OP_DC(NES* this);
uint32_t OP_DD(NES* this); uint32_t OP_DE(NES* this); uint32_t OP_DF(NES* this);

uint32_t OP_E0(NES* this); uint32_t OP_E1(NES* this); uint32_t OP_E2(NES* this); uint32_t OP_E3(NES* this);
uint32_t OP_E4(NES* this); uint32_t OP_E5(NES* this); uint32_t OP_E6(NES* this); uint32_t OP_E7(NES* this);
uint32_t OP_E8(NES* this); uint32_t OP_E9(NES* this); uint32_t OP_EA(NES* this); uint32_t OP_EB(NES* this);
uint32_t OP_EC(NES* this); uint32_t OP_ED(NES* this); uint32_t OP_EE(NES* this); uint32_t OP_EF(NES* this);

uint32_t OP_F0(NES* this); uint32_t OP_F1(NES* this); uint32_t OP_F3(NES* this); uint32_t OP_F4(NES* this);
uint32_t OP_F5(NES* this); uint32_t OP_F6(NES* this); uint32_t OP_F7(NES* this); uint32_t OP_F8(NES* this);
uint32_t OP_F9(NES* this); uint32_t OP_FA(NES* this); uint32_t OP_FB(NES* this); uint32_t OP_FC(NES* this);
uint32_t OP_FD(NES* this); uint32_t OP_FE(NES* this); uint32_t OP_FF(NES* this);

#endif