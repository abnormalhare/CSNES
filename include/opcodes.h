#ifndef OPCODES_H
#define OPCODES_H

#include "nes.h"
#include "rw.h"
#include "opfuncs.h"

#define CHECK_PAGE(f)     \
    if (page_crossed) {   \
        return f + 1;     \
    } else {              \
        return f;         \
    }

void OP_STP(NES* this);
void OP_00(NES* this); void OP_01(NES* this); void OP_03(NES* this); void OP_04(NES* this);
void OP_05(NES* this); void OP_06(NES* this); void OP_07(NES* this); void OP_08(NES* this);
void OP_09(NES* this); void OP_0A(NES* this); void OP_0B(NES* this); void OP_0C(NES* this);
void OP_0D(NES* this); void OP_0E(NES* this); void OP_0F(NES* this);

void OP_10(NES* this); void OP_11(NES* this); void OP_13(NES* this); void OP_14(NES* this);
void OP_15(NES* this); void OP_16(NES* this); void OP_17(NES* this); void OP_18(NES* this);
void OP_19(NES* this); void OP_1A(NES* this); void OP_1B(NES* this); void OP_1C(NES* this);
void OP_1D(NES* this); void OP_1E(NES* this); void OP_1F(NES* this);

void OP_20(NES* this); void OP_21(NES* this); void OP_23(NES* this); void OP_24(NES* this);
void OP_25(NES* this); void OP_26(NES* this); void OP_27(NES* this); void OP_28(NES* this);
void OP_29(NES* this); void OP_2A(NES* this); void OP_2B(NES* this); void OP_2C(NES* this);
void OP_2D(NES* this); void OP_2E(NES* this); void OP_2F(NES* this);

void OP_30(NES* this); void OP_31(NES* this); void OP_33(NES* this); void OP_34(NES* this);
void OP_35(NES* this); void OP_36(NES* this); void OP_37(NES* this); void OP_38(NES* this);
void OP_39(NES* this); void OP_3A(NES* this); void OP_3B(NES* this); void OP_3C(NES* this);
void OP_3D(NES* this); void OP_3E(NES* this); void OP_3F(NES* this);

void OP_40(NES* this); void OP_41(NES* this); void OP_43(NES* this); void OP_44(NES* this);
void OP_45(NES* this); void OP_46(NES* this); void OP_47(NES* this); void OP_48(NES* this);
void OP_49(NES* this); void OP_4A(NES* this); void OP_4B(NES* this); void OP_4C(NES* this);
void OP_4D(NES* this); void OP_4E(NES* this); void OP_4F(NES* this);

void OP_50(NES* this); void OP_51(NES* this); void OP_53(NES* this); void OP_54(NES* this);
void OP_55(NES* this); void OP_56(NES* this); void OP_57(NES* this); void OP_58(NES* this);
void OP_59(NES* this); void OP_5A(NES* this); void OP_5B(NES* this); void OP_5C(NES* this);
void OP_5D(NES* this); void OP_5E(NES* this); void OP_5F(NES* this);

void OP_60(NES* this); void OP_61(NES* this); void OP_63(NES* this); void OP_64(NES* this);
void OP_65(NES* this); void OP_66(NES* this); void OP_67(NES* this); void OP_68(NES* this);
void OP_69(NES* this); void OP_6A(NES* this); void OP_6B(NES* this); void OP_6C(NES* this);
void OP_6D(NES* this); void OP_6E(NES* this); void OP_6F(NES* this);

void OP_70(NES* this); void OP_71(NES* this); void OP_73(NES* this); void OP_74(NES* this);
void OP_75(NES* this); void OP_76(NES* this); void OP_77(NES* this); void OP_78(NES* this);
void OP_79(NES* this); void OP_7A(NES* this); void OP_7B(NES* this); void OP_7C(NES* this);
void OP_7D(NES* this); void OP_7E(NES* this); void OP_7F(NES* this);

void OP_80(NES* this); void OP_81(NES* this); void OP_82(NES* this); void OP_83(NES* this);
void OP_84(NES* this); void OP_85(NES* this); void OP_86(NES* this); void OP_87(NES* this);
void OP_88(NES* this); void OP_89(NES* this); void OP_8A(NES* this); void OP_8B(NES* this);
void OP_8C(NES* this); void OP_8D(NES* this); void OP_8E(NES* this); void OP_8F(NES* this);

void OP_90(NES* this); void OP_91(NES* this); void OP_93(NES* this); void OP_94(NES* this);
void OP_95(NES* this); void OP_96(NES* this); void OP_97(NES* this); void OP_98(NES* this);
void OP_99(NES* this); void OP_9A(NES* this); void OP_9B(NES* this); void OP_9C(NES* this);
void OP_9D(NES* this); void OP_9E(NES* this); void OP_9F(NES* this);

void OP_A0(NES* this); void OP_A1(NES* this); void OP_A2(NES* this); void OP_A3(NES* this);
void OP_A4(NES* this); void OP_A5(NES* this); void OP_A6(NES* this); void OP_A7(NES* this);
void OP_A8(NES* this); void OP_A9(NES* this); void OP_AA(NES* this); void OP_AB(NES* this);
void OP_AC(NES* this); void OP_AD(NES* this); void OP_AE(NES* this); void OP_AF(NES* this);

void OP_B0(NES* this); void OP_B1(NES* this); void OP_B3(NES* this); void OP_B4(NES* this);
void OP_B5(NES* this); void OP_B6(NES* this); void OP_B7(NES* this); void OP_B8(NES* this);
void OP_B9(NES* this); void OP_BA(NES* this); void OP_BB(NES* this); void OP_BC(NES* this);
void OP_BD(NES* this); void OP_BE(NES* this); void OP_BF(NES* this);

void OP_C0(NES* this); void OP_C1(NES* this); void OP_C2(NES* this); void OP_C3(NES* this);
void OP_C4(NES* this); void OP_C5(NES* this); void OP_C6(NES* this); void OP_C7(NES* this);
void OP_C8(NES* this); void OP_C9(NES* this); void OP_CA(NES* this); void OP_CB(NES* this);
void OP_CC(NES* this); void OP_CD(NES* this); void OP_CE(NES* this); void OP_CF(NES* this);

void OP_D0(NES* this); void OP_D1(NES* this); void OP_D3(NES* this); void OP_D4(NES* this);
void OP_D5(NES* this); void OP_D6(NES* this); void OP_D7(NES* this); void OP_D8(NES* this);
void OP_D9(NES* this); void OP_DA(NES* this); void OP_DB(NES* this); void OP_DC(NES* this);
void OP_DD(NES* this); void OP_DE(NES* this); void OP_DF(NES* this);

void OP_E0(NES* this); void OP_E1(NES* this); void OP_E2(NES* this); void OP_E3(NES* this);
void OP_E4(NES* this); void OP_E5(NES* this); void OP_E6(NES* this); void OP_E7(NES* this);
void OP_E8(NES* this); void OP_E9(NES* this); void OP_EA(NES* this); void OP_EB(NES* this);
void OP_EC(NES* this); void OP_ED(NES* this); void OP_EE(NES* this); void OP_EF(NES* this);

void OP_F0(NES* this); void OP_F1(NES* this); void OP_F3(NES* this); void OP_F4(NES* this);
void OP_F5(NES* this); void OP_F6(NES* this); void OP_F7(NES* this); void OP_F8(NES* this);
void OP_F9(NES* this); void OP_FA(NES* this); void OP_FB(NES* this); void OP_FC(NES* this);
void OP_FD(NES* this); void OP_FE(NES* this); void OP_FF(NES* this);

#endif