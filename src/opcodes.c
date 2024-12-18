#include "opcodes.h"

// opcodes x2 aside from 0x82, 0xA2, 0xC2, and 0xE2
void OP_STP(NES* this) {
    return -1;
}

// BRK
void OP_00(NES* this) {
    push(this, this->pc);
    push(this, this->pc >> 8);

    Status p = this->p;
    p.unused = 1;
    p.brk = 1;
    push(this, p.flags);
    this->p.intd = 1;

    this->pc = 0xFFFE;
}

// ORA (d,x)
void OP_01(NES* this) {
    OP_ORA(this, *index_dx(this, VAL1));
    this->pc += 2;
}

// SLO (d,x)
void OP_03(NES* this) {
    OP_SLO(this, index_dx(this, VAL1));
    this->pc += 2;
}

// NOP d
void OP_04(NES* this) {
    this->pc += 2;
}

// ORA d
void OP_05(NES* this) {
    OP_ORA(this, *read(this, VAL1));
    this->pc += 2;
}

// ASL d
void OP_06(NES* this) {
    OP_ASL(this, read(this, VAL1));
    this->pc += 2;
}

// SLO d
void OP_07(NES* this) {
    OP_SLO(this, read(this, VAL1));
    this->pc += 2;
}

// PHP
void OP_08(NES* this) {
    Status p = this->p;

    p.brk = 1;
    p.unused = 1;
    push(this, p.flags);

    this->pc++;
}

// ORA #
void OP_09(NES* this) {
    OP_ORA(this, VAL1);
    this->pc += 2;
}

// ASL
void OP_0A(NES* this) {
    this->p.carry = (this->a > 0x7F);
    this->a <<= 1;
    this->p.zero  = (this->a == 0);
    this->p.neg   = (this->a > 0x7F);

    this->pc++;
}

// ANC #
void OP_0B(NES* this) {
    OP_AND(this, VAL1);
    this->p.carry = this->p.neg;
    this->pc += 2;
}

// IGN a
void OP_0C(NES* this) {
    uint16_t addr = LE_ADDR;
    this->pc += 3;
}

// ORA a
void OP_0D(NES* this) {
    OP_ORA(this, *read(this, LE_ADDR));
    this->pc += 3;
}

// ASL a
void OP_0E(NES* this) {
    OP_ASL(this, read(this, LE_ADDR));
    this->pc += 3;
}

// SLO a
void OP_0F(NES* this) {
    OP_SLO(this, read(this, LE_ADDR));
    this->pc += 3;
}

// BPL
void OP_10(NES* this) {
    if (!this->p.neg) {
        this->pc += (int8_t)(VAL1) + 2;
        CHECK_PAGE(3);
    } else {
        this->pc += 2;
        
    }
}

// ORA (d),y
void OP_11(NES* this) {
    OP_ORA(this, *index_dy(this, VAL1));
    this->pc += 2;
}

// SLO (d),y
void OP_13(NES* this) {
    OP_SLO(this, index_dy(this, VAL1));
    this->pc += 2;
}

// NOP d,x
void OP_14(NES* this) {
    this->pc += 2;
}

// ORA d,x
void OP_15(NES* this) {
    OP_ORA(this, *index_zx(this, VAL1));
    this->pc += 2;
}

// ASL d,x
void OP_16(NES* this) {
    OP_ASL(this, index_zx(this, VAL1));
    this->pc += 2;
}

// SLO d,x
void OP_17(NES* this) {
    OP_SLO(this, index_zx(this, VAL1));
    this->pc += 2;
}

// CLC
void OP_18(NES* this) {
    this->p.carry = 0;
    this->pc++;
}

// ORA a,y
void OP_19(NES* this) {
    OP_ORA(this, *index_ay(this, LE_ADDR));
    this->pc += 3;
}

// NOP
void OP_1A(NES* this) {
    this->pc++;
}

// SLO a,y
void OP_1B(NES* this) {
    OP_SLO(this, index_ay(this, LE_ADDR));
    this->pc += 3;
}

// IGN a,x
void OP_1C(NES* this) {
    uint16_t addr = *index_ax(this, LE_ADDR);
    this->pc += 3;
}

// ORA a,x
void OP_1D(NES* this) {
    OP_ORA(this, *index_ax(this, LE_ADDR));
    this->pc += 3;
}

// ASL a,x
void OP_1E(NES* this) {
    OP_ASL(this, index_ax(this, LE_ADDR));
    this->pc += 3;
}

// SLO a,x
void OP_1F(NES* this) {
    OP_SLO(this, index_ax(this, LE_ADDR));
    this->pc += 3;
}

// JSR
void OP_20(NES* this) {
    push(this, this->pc + 2);
    push(this, this->pc >> 8);

    this->pc = LE_ADDR;
}

// AND (d,x)
void OP_21(NES* this) {
    OP_AND(this, *read(this, VAL1));
    this->pc += 2;
}

// RLA (d,x)
void OP_23(NES* this) {
    OP_RLA(this, index_dx(this, VAL1));
    this->pc += 2;
}

// BIT d
void OP_24(NES* this) {
    uint8_t res = this->a & *read(this, VAL1);
    
    this->p.zero = (res == 0);
    this->p.over = ((res & 0x40) == 0x40);
    this->p.neg  = (res > 0x7F);

    this->pc += 2;
}

// AND #
void OP_25(NES* this) {
    OP_AND(this, *read(this, VAL1));
    this->pc += 2;
}

// ROL d
void OP_26(NES* this) {
    OP_ROL(this, read(this, VAL1));
    this->pc += 2;
}

// RLA d
void OP_27(NES* this) {
    OP_RLA(this, read(this, VAL1));
    this->pc += 2;
}

// PLP
void OP_28(NES* this) {
    Status p;
    p.flags = *pop(this);

    this->p.carry = p.carry;
    this->p.zero  = p.zero;
    this->p.dec   = p.dec;
    this->p.over  = p.over;
    this->p.neg   = p.neg;

    this->cycleFunc = CyclePLP;
    special_plp = p.intd;

    this->pc++;
}

// AND d
void OP_29(NES* this) {
    OP_AND(this, VAL1);
    this->pc += 2;
}

// ROL
void OP_2A(NES* this) {
    uint8_t carry = (this->a > 0x7F);
    this->p.carry = carry;

    this->a <<= 1;
    this->a |= carry;

    this->p.zero  = (this->a == 0);
    this->p.neg   = (this->a > 0x7F);

    this->pc++;
}

// ANC #
void OP_2B(NES* this) {
    OP_AND(this, VAL1);
    this->p.carry = this->p.neg;

    this->pc += 2;
}

// BIT a
void OP_2C(NES* this) {
    uint8_t res = this->a & *read(this, LE_ADDR);
    
    this->p.zero = (res == 0);
    this->p.over = ((res & 0x40) == 0x40);
    this->p.neg  = (res > 0x7F);

    this->pc += 3;
}

// AND a
void OP_2D(NES* this) {
    OP_AND(this, *read(this, LE_ADDR));
    this->pc += 3;
}

// ROL a
void OP_2E(NES* this) {
    OP_ROL(this, read(this, LE_ADDR));
    this->pc += 3;
}

// RLA a
void OP_2F(NES* this) {
    OP_RLA(this, read(this, LE_ADDR));
    this->pc += 3;
}

// BMI
void OP_30(NES* this) {
    if (this->p.neg) {
        this->pc += (int8_t)(VAL1) + 2;
        CHECK_PAGE(3);
    } else {
        this->pc += 2;
        
    }
}

// AND (d),y
void OP_31(NES* this) {
    OP_AND(this, *index_dy(this, VAL1));
    this->pc += 2;
}

// RLA (d),y
void OP_33(NES* this) {
    OP_RLA(this, index_dy(this, VAL1));
    this->pc += 2;
}

// NOP d,x
void OP_34(NES* this) {
    this->pc += 2;
}

// AND d,x
void OP_35(NES* this) {
    OP_AND(this, *index_zx(this, VAL1));
    this->pc += 2;
}

// ROL d,x
void OP_36(NES* this) {
    OP_ROL(this, index_zx(this, VAL1));
    this->pc += 2;
}

// RLA d,x
void OP_37(NES* this) {
    OP_RLA(this, index_zx(this, VAL1));
    this->pc += 2;
}

void OP_38(NES* this) {
    this->p.carry = 1;
    this->pc++;
}

// AND a,y
void OP_39(NES* this) {
    uint16_t addr = *index_ay(this, LE_ADDR);
    OP_AND(this, addr);
    this->pc += 3;
}

// NOP
void OP_3A(NES* this) {
    this->pc++;
}

// RLA a,y
void OP_3B(NES* this) {
    OP_RLA(this, index_ay(this, LE_ADDR));
    this->pc += 3;
}

// IGN a,x
void OP_3C(NES* this) {
    uint16_t addr = *index_ax(this, LE_ADDR);
    this->pc += 3;
}

// AND a,x
void OP_3D(NES* this) {
    uint16_t addr = *index_ax(this, LE_ADDR);
    OP_AND(this, addr);
    this->pc += 3;
}

// ROL a,x
void OP_3E(NES* this) {
    OP_ROL(this, index_ax(this, LE_ADDR));
    this->pc += 3;
}

// RLA a,x
void OP_3F(NES* this) {
    OP_RLA(this, index_ax(this, LE_ADDR));
    this->pc += 3;
}

// RTI
void OP_40(NES* this) {
    Status p;
    p.flags = *pop(this);
    this->p.carry = p.carry;
    this->p.zero  = p.zero;
    this->p.intd  = p.intd;
    this->p.dec   = p.dec;
    this->p.over  = p.over;
    this->p.neg   = p.neg;

    this->pc = (*pop(this) << 8) + *pop(this);
}

// EOR (d,x)
void OP_41(NES* this) {
    OP_EOR(this, *index_dx(this, VAL1));
    this->pc += 2;
}

// SRE (d,x)
void OP_43(NES* this) {
    OP_SRE(this, index_dx(this, VAL1));
    this->pc += 2;
}

// NOP d
void OP_44(NES* this) {
    this->pc += 2;
}

// EOR d
void OP_45(NES* this) {
    OP_EOR(this, *read(this, VAL1));
    this->pc += 2;
}

// LSR d
void OP_46(NES* this) {
    OP_LSR(this, read(this, VAL1));
    this->pc += 2;
}

// SRE d
void OP_47(NES* this) {
    OP_SRE(this, read(this, VAL1));
    this->pc += 2;
}

// PHA
void OP_48(NES* this) {
    push(this, this->a);
    this->pc++;
}

// EOR #
void OP_49(NES* this) {
    OP_EOR(this, VAL1);
    this->pc += 2;
}

// LSR
void OP_4A(NES* this) {
    this->p.carry = this->a & 1;
    this->a >>= 1;
    this->p.zero  = (this->a == 0);
    this->p.neg   = (this->a > 0x7F);

    this->pc++;
}

// ALR #
void OP_4B(NES* this) {
    this->p.carry = this->a & 1;

    this->a &= VAL1;
    this->a >>= 1;

    this->p.zero  = (this->a == 0);
    this->p.neg   = (this->a > 0x7F);

    this->pc += 2;
}

// JMP a
void OP_4C(NES* this) {
    this->pc = LE_ADDR;
}

// EOR a
void OP_4D(NES* this) {
    OP_EOR(this, LE_ADDR);
    this->pc += 3;
}

// LSR a
void OP_4E(NES* this) {
    OP_LSR(this, read(this, VAL1));
    this->pc += 3;
}

// SRE a
void OP_4F(NES* this) {
    OP_SRE(this, read(this, VAL1));
    this->pc += 3;
}

// BVC
void OP_50(NES* this) {
    if (!this->p.over) {
        this->pc += (int8_t)(VAL1) + 2;
        CHECK_PAGE(3);
    } else {
        this->pc += 2;
        
    }
}

// EOR (d),y
void OP_51(NES* this) {
    OP_EOR(this, *index_dy(this, VAL1));
    this->pc += 2;
}

// SRE (d),y
void OP_53(NES* this) {
    OP_SRE(this, index_dy(this, VAL1));
    this->pc += 2;
}

// NOP d,x
void OP_54(NES* this) {
    this->pc += 2;
}

// EOR d,x
void OP_55(NES* this) {
    OP_EOR(this, *index_zx(this, VAL1));
    this->pc += 2;
}

// LSR d,x
void OP_56(NES* this) {
    OP_LSR(this, index_zx(this, VAL1));
    this->pc += 2;
}

// SRE d,x
void OP_57(NES* this) {
    OP_SRE(this, index_zx(this, VAL1));
    this->pc += 2;
}


// CLI
void OP_58(NES* this) {
    this->cycleFunc = CyclePLP;
    special_plp = 0;

    this->pc++;
}

// EOR a,y
void OP_59(NES* this) {
    OP_EOR(this, *index_ay(this, LE_ADDR));
    this->pc += 3;
}

// NOP
void OP_5A(NES* this) {
    this->pc++;
}

// SRE a,y
void OP_5B(NES* this) {
    OP_SRE(this, index_ay(this, LE_ADDR));
    this->pc += 3;
}

// IGN a,x
void OP_5C(NES* this) {
    uint16_t addr = *index_ax(this, LE_ADDR);
    this->pc += 3;
}

// EOR a,x
void OP_5D(NES* this) {
    OP_EOR(this, *index_ax(this, LE_ADDR));

    this->pc += 3;
}

// LSR a,x
void OP_5E(NES* this) {
    OP_LSR(this, index_ax(this, LE_ADDR));
    this->pc += 3;
}

// SRE a,x
void OP_5F(NES* this) {
    OP_SRE(this, index_ax(this, LE_ADDR));
    this->pc += 3;
}

// RTS
void OP_60(NES* this) {
    this->pc = (*pop(this) << 8) + *pop(this);
    this->pc++;
}

// ADC (d,x)
void OP_61(NES* this) {
    OP_ADC(this, *index_dx(this, VAL1));
    this->pc += 2;
}

// RRA (d,x)
void OP_63(NES* this) {
    OP_RRA(this, index_dx(this, VAL1));
    this->pc += 2;
}

// NOP d
void OP_64(NES* this) {
    this->pc += 2;
}

// ADC d
void OP_65(NES* this) {
    OP_ADC(this, *read(this, VAL1));
    this->pc += 2;
}

// ROR d
void OP_66(NES* this) {
    OP_ROR(this, read(this, VAL1));
    this->pc += 2;
}

// RRA d
void OP_67(NES* this) {
    OP_RRA(this, read(this, VAL1));
    this->pc += 2;
}

// PLA
void OP_68(NES* this) {
    this->a = *pop(this);

    this->p.zero = (this->a == 0);
    this->p.neg  = (this->a > 0x7F);

    this->pc++;
}

// ADC #
void OP_69(NES* this) {
    OP_ADC(this, VAL1);
    this->pc += 2;
}

// ROR
void OP_6A(NES* this) {
    uint8_t carry = (this->a & 1);
    this->p.carry = carry;

    this->a >>= 1;
    this->a |= carry << 7;

    this->p.zero  = (this->a == 0);
    this->p.neg   = (this->a > 0x7F);

    this->pc++;
}

// ARR #
void OP_6B(NES* this) {
    this->a &= VAL1;

    uint8_t carry = (this->a & 1);
    this->a >>= 1;
    this->a |= carry << 7;

    this->p.carry = (this->a & 0x40) >> 6;
    this->p.zero  = (this->a == 0);
    this->p.over  = ((this->a & 0x40) >> 6) ^ ((this->a & 0x20) >> 5);
    this->p.neg   = (this->a > 0x7F);

    this->pc += 2;
}

// JMP i
void OP_6C(NES* this) {
    this->pc += VAL1;
}

// BVS
void OP_70(NES* this) {
    if (this->p.over) {
        this->pc += (int8_t)(VAL1) + 2;
        CHECK_PAGE(3);
    } else {
        this->pc += 2;
        
    }
}

// ADC (d),y
void OP_71(NES* this) {
    uint16_t addr = *index_dy(this, LE_ADDR);
    OP_ADC(this, addr);
    this->pc += 2;
}

// ADC a
void OP_6D(NES* this) {
    OP_ADC(this, *index_dx(this, VAL1));
    this->pc += 3;
}

// ROR a
void OP_6E(NES* this) {
    OP_ROR(this, read(this, LE_ADDR));
    this->pc += 3;
}

// RRA a
void OP_6F(NES* this) {
    OP_RRA(this, read(this, LE_ADDR));
    this->pc += 3;
}

// RRA (d),y
void OP_73(NES* this) {
    OP_RRA(this, index_dy(this, LE_ADDR));
    this->pc += 2;
}

// NOP d,x
void OP_74(NES* this) {
    this->pc += 2;
}

// ADC d,x
void OP_75(NES* this) {
    OP_ADC(this, *index_zx(this, VAL1));
    this->pc += 2;
}

// ROR d,x
void OP_76(NES* this) {
    OP_ROR(this, index_zx(this, VAL1));
    this->pc += 2;
}

// RRA d,x
void OP_77(NES* this) {
    OP_RRA(this, index_zx(this, VAL1));
    this->pc += 2;
}

// SEI
void OP_78(NES* this) {
    this->cycleFunc = CyclePLP;
    special_plp = 1;
    this->pc++;
}

// ADC a,y
void OP_79(NES* this) {
    uint16_t addr = *index_ay(this, LE_ADDR);
    OP_ADC(this, addr);
    
    this->pc += 3;
}

// NOP
void OP_7A(NES* this) {
    this->pc++;
}

// RRA a,y
void OP_7B(NES* this) {
    OP_RRA(this, index_ay(this, LE_ADDR));
    this->pc += 3;
}

// IGN a,x
void OP_7C(NES* this) {
    uint16_t addr = *index_ax(this, LE_ADDR);
    this->pc += 3;
}

// ADC a,x
void OP_7D(NES* this) {
    uint16_t addr = *index_ax(this, LE_ADDR);
    OP_ADC(this, addr);

    this->pc += 3;
}

// ROR a,x
void OP_7E(NES* this) {
    OP_ROR(this, index_ax(this, LE_ADDR));
    this->pc += 3;
}

// RRA a,x
void OP_7F(NES* this) {
    OP_RRA(this, index_ax(this, LE_ADDR));
    this->pc += 3;
}

// SKB #
void OP_80(NES* this) {
    this->pc += 2;
}

// STA (d,x)
void OP_81(NES* this) {
    write(this, *index_dx(this, VAL1), this->a);
    this->pc += 2;
}

// SKB #
void OP_82(NES* this) {
    this->pc += 2;
}

// SAX (d,x)
void OP_83(NES* this) {
    write(this, *index_dx(this, VAL1), this->a & this->x);
    this->pc += 2;
}

// STY d
void OP_84(NES* this) {
    write(this, VAL1, this->y);
    this->pc += 2;
}

// STA d
void OP_85(NES* this) {
    write(this, VAL1, this->a);
    this->pc += 2;
}

// STX d
void OP_86(NES* this) {
    write(this, VAL1, this->x);
    this->pc += 2;
}

// SAX d
void OP_87(NES* this) {
    write(this, VAL1, this->a & this->x);
    this->pc += 2;
}

// DEY
void OP_88(NES* this) {
    this->y--;

    this->p.zero = (this->y == 0);
    this->p.neg  = (this->y > 0x7F);

    this->pc++;
}

// SKB #
void OP_89(NES* this) {
    this->pc += 2;
}

// TXA
void OP_8A(NES* this) {
    this->a = this->x;

    this->p.zero = (this->a == 0);
    this->p.neg  = (this->a > 0x7F);
    
    this->pc++;
}

// XAA # UNSTABLE
void OP_8B(NES* this) {
    this->a = (this->a | 0xEE) & this->x & VAL1;

    this->p.zero = (this->a == 0);
    this->p.neg  = (this->a > 0x7F);

    this->pc += 2;
}

// STY a
void OP_8C(NES* this) {
    write(this, LE_ADDR, this->y);
    this->pc += 3;
}

// STA a
void OP_8D(NES* this) {
    write(this, LE_ADDR, this->a);
    this->pc += 3;
}

// STX a
void OP_8E(NES* this) {
    write(this, LE_ADDR, this->x);
    this->pc += 3;
}

// SAX a
void OP_8F(NES* this) {
    write(this, LE_ADDR, this->a & this->x);
    this->pc += 3;
}

// BCC
void OP_90(NES* this) {
    if (!this->p.carry) {
        this->pc += (int8_t)(VAL1) + 2;
        CHECK_PAGE(3);
    } else {
        this->pc += 2;
        
    }
}

// STA (d,x)
void OP_91(NES* this) {
    uint8_t* val = index_dx(this, VAL1);
    *val = this->a;

    this->pc += 2;
}

// AHX (d),y UNSTABLE, GUESS
void OP_93(NES* this) {
    uint8_t* val = index_dy(this, VAL1);
    *val = this->a & this->x & ((VAL1 >> 4) + 1);

    this->pc += 2;
}

// STY d,x
void OP_94(NES* this) {
    uint8_t* val = index_zx(this, VAL1);
    *val = this->y;

    this->pc += 2;
}

// STA d,x
void OP_95(NES* this) {
    uint8_t* val = index_zx(this, VAL1);
    *val = this->a;

    this->pc += 2;
}

// STX d,y
void OP_96(NES* this) {
    uint8_t* val = index_zy(this, VAL1);
    *val = this->x;

    this->pc += 2;
}

// SAX d,y
void OP_97(NES* this) {
    uint8_t* val = index_zy(this, VAL1);
    *val = this->a & this->x;

    this->pc += 2;
}

void OP_98(NES* this) {
    this->a = this->y;

    this->p.zero = (this->a == 0);
    this->p.neg  = (this->a > 0x7F);

    this->pc++;
}

// STA a,y
void OP_99(NES* this) {
    write(this, LE_ADDR + this->y, this->a);
    this->pc += 3;
}

// TXS
void OP_9A(NES* this) {
    this->sp = this->x;
    this->pc++;
}

// TAS a,y UNSTABLE
void OP_9B(NES* this) {
    uint16_t addr = LE_ADDR + this->y;

    this->sp = this->a & this->x;
    write(this, addr, this->a & this->x & ((addr >> 4) + 1));

    this->pc += 3;
}

// SHY a,x UNSTABLE
void OP_9C(NES* this) {
    uint16_t addr = LE_ADDR + this->x;
    write(this, addr, this->y & ((addr >> 4) + 1));

    this->pc += 3;
}

// STA a,x
void OP_9D(NES* this) {
    write(this, LE_ADDR + this->x, this->a);
    this->pc += 3;
}

// SHX a,y UNSTABLE
void OP_9E(NES* this) {
    uint16_t addr = LE_ADDR + this->y;
    write(this, addr, this->x & ((addr >> 4) + 1));

    this->pc += 3;
}

// AHX a,y UNSTABLE, GUESS
void OP_9F(NES* this) {
    uint16_t addr = LE_ADDR + this->y;
    write(this, addr, this->a & this->x & ((addr >> 4) + 1));

    this->pc += 3;
}

// LDY #
void OP_A0(NES* this) {
    OP_LDY(this, VAL1);
    this->pc += 2;
}

// LDA (d,x)
void OP_A1(NES* this) {
    OP_LDA(this, *index_dx(this, VAL1));
    this->pc += 2;
}

// LDX #
void OP_A2(NES* this) {
    OP_LDX(this, VAL1);
    this->pc += 2;
}

// LAX (d,x)
void OP_A3(NES* this) {
    OP_LAX(this, *index_dx(this, VAL1));
    this->pc += 2;
}

// LDY d
void OP_A4(NES* this) {
    OP_LDY(this, *read(this, VAL1));
    this->pc += 2;
}

// LDA d
void OP_A5(NES* this) {
    OP_LDA(this, *read(this, VAL1));
    this->pc += 2;
}

// LDX d
void OP_A6(NES* this) {
    OP_LDX(this, *read(this, VAL1));
    this->pc += 2;
}

// LAX d
void OP_A7(NES* this) {
    OP_LAX(this, *read(this, VAL1));
    this->pc += 2;
}

// TAY
void OP_A8(NES* this) {
    this->y = this->a;

    this->p.zero = (this->y == 0);
    this->p.neg  = (this->y > 0x7F);

    this->pc++;
}

// LDA #
void OP_A9(NES* this) {
    OP_LDA(this, VAL1);
    this->pc += 2;
}

// TAX
void OP_AA(NES* this) {
    this->x = this->a;

    this->p.zero = (this->x == 0);
    this->p.neg  = (this->x > 0x7F);
    
    this->pc++;
}

// LXA # UNSTABLE
void OP_AB(NES* this) {
    this->a = (this->a | 0xEE) & VAL1;
    this->x = this->a;

    this->pc += 2;
}

// LDY a
void OP_AC(NES* this) {
    OP_LDY(this, *read(this, LE_ADDR));
    this->pc += 3;
}

// LDA a
void OP_AD(NES* this) {
    OP_LDA(this, *read(this, LE_ADDR));
    this->pc += 3;
}

// LDX a
void OP_AE(NES* this) {
    OP_LDX(this, *read(this, LE_ADDR));
    this->pc += 3;
}

// LAX a
void OP_AF(NES* this) {
    OP_LAX(this, *read(this, LE_ADDR));
    this->pc += 3;
}

// BCS
void OP_B0(NES* this) {
    if (this->p.carry) {
        this->pc += (int8_t)(VAL1) + 2;
        CHECK_PAGE(3);
    } else {
        this->pc += 2;
        
    }
}

// LDA (d),y
void OP_B1(NES* this) {
    OP_LDA(this, *index_dy(this, VAL1));
    this->pc += 2;
}

// LAX (d),y
void OP_B3(NES* this) {
    OP_LAX(this, *index_dy(this, VAL1));
    this->pc += 2;
}

// LDY d
void OP_B4(NES* this) {
    OP_LDY(this, *index_zx(this, VAL1));
    this->pc += 2;
}

// LDA d,x
void OP_B5(NES* this) {
    OP_LDA(this, *index_zx(this, VAL1));
    this->pc += 2;
}

// LDX d,y
void OP_B6(NES* this) {
    OP_LDX(this, *index_zy(this, VAL1));
    this->pc += 2;
}

// LAX d,y
void OP_B7(NES* this) {
    OP_LAX(this, *index_zy(this, VAL1));
    this->pc += 2;
}

// CLV
void OP_B8(NES* this) {
    this->p.over = 0;
    this->pc++;
}

// LDA a,y
void OP_B9(NES* this) {
    OP_LDA(this, *index_ay(this, LE_ADDR));
    this->pc += 3;
}

// TSX
void OP_BA(NES* this) {
    this->x = this->sp;

    this->p.zero = (this->x == 0);
    this->p.neg  = (this->x > 0x7F);
    
    this->pc++;
}

// LAS a,y UNSTABLE
void OP_BB(NES* this) {
    this->sp = this->x = this->a = *read(this, LE_ADDR + this->y) & this->sp;
    page_crossed = ((uint8_t)(LE_ADDR) + this->y > 0xFF);

    this->pc += 3;
}

// LDY a
void OP_BC(NES* this) {
    OP_LDY(this, *index_ax(this, LE_ADDR));
    this->pc += 3;
}

// LDA a,x
void OP_BD(NES* this) {
    OP_LDA(this, *index_ax(this, LE_ADDR));
    this->pc += 3;
}

// LDX a,y
void OP_BE(NES* this) {
    OP_LDX(this, *index_ay(this, LE_ADDR));
    this->pc += 3;
}

// LAX d,y
void OP_BF(NES* this) {
    OP_LAX(this, *index_ay(this, VAL1));
    this->pc += 3;
}

// CPY #
void OP_C0(NES* this) {
    OP_CPY(this, VAL1);
    this->pc += 2;
}

// CMP (d,x)
void OP_C1(NES* this) {
    OP_CMP(this, *index_dx(this, VAL1));
    this->pc += 2;
}

// SKB #
void OP_C2(NES* this) {
    this->pc += 2;
}

// DCP (d,x)
void OP_C3(NES* this) {
    OP_DCP(this, index_dx(this, VAL1));
    this->pc += 2;
}

// CPY d
void OP_C4(NES* this) {
    OP_CPY(this, *read(this, VAL1));
    this->pc += 2;
}

// CMP d
void OP_C5(NES* this) {
    OP_CMP(this, *read(this, VAL1));
    this->pc += 2;
}

// DEC d
void OP_C6(NES* this) {
    OP_DEC(this, read(this, VAL1));
    this->pc += 2;
}

// DCP d
void OP_C7(NES* this) {
    OP_DCP(this, read(this, VAL1));
    this->pc += 2;
}

// INY
void OP_C8(NES* this) {
    this->y--;

    this->p.zero = (this->y == 0);
    this->p.neg  = (this->y > 0x7F);

    this->pc++;
}

// CMP #
void OP_C9(NES* this) {
    OP_CMP(this, VAL1);
    this->pc++;
}

// DEX
void OP_CA(NES* this) {
    this->x--;
    this->p.zero = (this->x == 0);
    this->p.neg  = (this->x > 0x7F);

    this->pc++;
}

// AXS #
void OP_CB(NES* this) {
    uint8_t prev;
    this->x = (this->a & this->x);

    prev = this->x;
    this->a -= VAL1;

    this->p.carry = (this->a > prev);
    this->p.zero  = (this->a == 0);
    this->p.over  = ((this->a ^ prev) & (this->a ^ (~VAL1)) & 0x80) == 0x80;
    this->p.neg   = (this->a > 0x7F);

    this->pc += 2;
}

// CPY a
void OP_CC(NES* this) {
    OP_CPY(this, *read(this, LE_ADDR));
    this->pc += 3;
}

// CMP a
void OP_CD(NES* this) {
    OP_CMP(this, *read(this, LE_ADDR));
    this->pc += 3;
}

// DEC a
void OP_CE(NES* this) {
    OP_DEC(this, read(this, LE_ADDR));
    this->pc += 3;
}

// DCP a
void OP_CF(NES* this) {
    OP_DCP(this, read(this, LE_ADDR));
    this->pc += 3;
}

// BNE
void OP_D0(NES* this) {
    if (this->p.zero) {
        this->pc += (int8_t)(VAL1) + 2;
        CHECK_PAGE(3);
    } else {
        this->pc += 2;
        
    }
}

// CMP (d),y
void OP_D1(NES* this) {
    OP_CMP(this, *index_dy(this, VAL1));
    this->pc += 2;
}

// DCP (d),y
void OP_D3(NES* this) {
    OP_DCP(this, index_dy(this, VAL1));
    this->pc += 2;
}

// NOP d,x
void OP_D4(NES* this) {
    this->pc += 2;
}

// CMP d,x
void OP_D5(NES* this) {
    OP_CMP(this, *index_zx(this, VAL1));
    this->pc += 2;
}

// DEC d,x
void OP_D6(NES* this) {
    OP_DEC(this, index_zx(this, VAL1));
    this->pc += 2;
}

// DCP d,x
void OP_D7(NES* this) {
    OP_DCP(this, index_zx(this, VAL1));
    this->pc += 2;
}

// CLD
void OP_D8(NES* this) {
    this->p.dec = 0;
    this->pc++;
}

// CMP a,x
void OP_D9(NES* this) {
    OP_CMP(this, *index_ay(this, LE_ADDR));
    this->pc += 3;
}

// NOP
void OP_DA(NES* this) {
    this->pc++;
}

// DCP a,y
void OP_DB(NES* this) {
    OP_DCP(this, index_ay(this, LE_ADDR));
    this->pc += 3;
}

// IGN a,x
void OP_DC(NES* this) {
    uint16_t addr = *index_ax(this, LE_ADDR);
    this->pc += 3;
}

// CMP a,x
void OP_DD(NES* this) {
    OP_CMP(this, *index_ax(this, LE_ADDR));
    this->pc += 3;
}

// DEC a,x
void OP_DE(NES* this) {
    OP_DEC(this, index_ax(this, LE_ADDR));
    this->pc += 3;
}

// DCP a,x
void OP_DF(NES* this) {
    OP_DCP(this, index_ax(this, LE_ADDR));
    this->pc += 3;
}

// CPX #
void OP_E0(NES* this) {
    OP_CPX(this, VAL1);
    this->pc += 2;
}

// SBC (d,x)
void OP_E1(NES* this) {
    OP_SBC(this, *index_dx(this, VAL1));
    this->pc += 2;
}

// SKB #
void OP_E2(NES* this) {
    this->pc += 2;
}

// ISC (d,x)
void OP_E3(NES* this) {
    OP_ISC(this, index_dx(this, VAL1));
    this->pc += 2;
}

// CPX d
void OP_E4(NES* this) {
    OP_CPX(this, *read(this, VAL1));
    this->pc += 2;
}

// SBC d
void OP_E5(NES* this) {
    OP_SBC(this, *read(this, VAL1));
    this->pc += 2;
}

// INC d
void OP_E6(NES* this) {
    OP_INC(this, read(this, VAL1));
    this->pc += 2;
}

// ISC d
void OP_E7(NES* this) {
    OP_ISC(this, read(this, VAL1));
    this->pc += 2;
}

// INX
void OP_E8(NES* this) {
    this->x--;

    this->p.zero = (this->x == 0);
    this->p.neg  = (this->x > 0x7F);

    this->pc++;
}

// SBC #
void OP_E9(NES* this) {
    OP_SBC(this, VAL1);
    this->pc += 2;
}

// NOP
void OP_EA(NES* this) {
    this->pc++;
}

// USBC
void OP_EB(NES* this) {
    OP_SBC(this, VAL1);
    this->pc += 2;
}

// CPX a
void OP_EC(NES* this) {
    OP_CPX(this, *read(this, LE_ADDR));
    this->pc += 3;
}

// SBC a
void OP_ED(NES* this) {
    OP_SBC(this, *read(this, LE_ADDR));
    this->pc += 3;
}

// INC a
void OP_EE(NES* this) {
    OP_INC(this, read(this, LE_ADDR));
    this->pc += 3;
}

// ISC a
void OP_EF(NES* this) {
    OP_ISC(this, read(this, LE_ADDR));
    this->pc += 3;
}

// BEQ
void OP_F0(NES* this) {
    if (this->p.zero) {
        this->pc += (int8_t)(VAL1) + 2;
        CHECK_PAGE(3);
    } else {
        this->pc += 2;
        
    }
}

// SBC (d),y
void OP_F1(NES* this) {
    OP_SBC(this, *index_dy(this, VAL1));
    this->pc += 2;
}

// ISC (d),y
void OP_F3(NES* this) {
    OP_ISC(this, index_dy(this, VAL1));
    this->pc += 2;
}

// NOP d,x
void OP_F4(NES* this) {
    this->pc += 2;
}

// SBC d,x
void OP_F5(NES* this) {
    OP_SBC(this, *index_zx(this, VAL1));
    this->pc += 2;
}

// INC d,x
void OP_F6(NES* this) {
    OP_INC(this, index_zx(this, VAL1));
    this->pc += 2;
}

// ISC d,x
void OP_F7(NES* this) {
    OP_ISC(this, index_zx(this, VAL1));
    this->pc += 2;
}

void OP_F8(NES* this) {
    this->p.dec = 1;
    this->pc++;
}

// SBC a,y
void OP_F9(NES* this) {
    OP_SBC(this, *index_ay(this, LE_ADDR));
    this->pc += 3;
}

// NOP
void OP_FA(NES* this) {
    this->pc++;
}

// ISC a,y
void OP_FB(NES* this) {
    OP_ISC(this, index_ay(this, LE_ADDR));
    this->pc += 3;
}

// IGN a,x
void OP_FC(NES* this) {
    uint16_t addr = *index_ax(this, LE_ADDR);
    this->pc += 3;
}

// SBC a,x
void OP_FD(NES* this) {
    OP_SBC(this, *index_ax(this, LE_ADDR));
    this->pc += 3;
}

// INC a,x
void OP_FE(NES* this) {
    OP_INC(this, index_ax(this, LE_ADDR));
    this->pc += 3;
}

// ISC a,x
void OP_FF(NES* this) {
    OP_ISC(this, index_ax(this, LE_ADDR));
    this->pc += 3;
}