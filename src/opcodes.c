#include "opcodes.h"

// opcodes x2 aside from 0x82, 0xA2, 0xC2, and 0xE2
uint32_t OP_STP(NES* this) {
    return -1;
}

// BRK
uint32_t OP_00(NES* this) {
    push(this, this->pc);
    push(this, this->pc >> 8);

    Status p = this->p;
    p.unused = 1;
    p.brk = 1;
    push(this, p.flags);
    this->p.intd = 1;

    this->pc = 0xFFFE;
    return 7;
}

// ORA (d,x)
uint32_t OP_01(NES* this) {
    OP_ORA(this, index_dx(this, VAL1));

    this->pc += 2;
    return 6;
}

// SLO (d,x)
uint32_t OP_03(NES* this) {
    uint8_t value = index_dx(this, VAL1);

    this->p.carry = (this->a > 0x7F);

    this->a <<= 1;
    this->a |= value;

    this->p.zero = (this->a == 0);
    this->p.neg  = (this->a > 0x7F);

    this->pc += 2;
    return 8;
}

// NOP d
uint32_t OP_04(NES* this) {
    this->pc += 2;
    return 3;
}

// ORA d
uint32_t OP_05(NES* this) {
    OP_ORA(this, this->RAM[VAL1]);

    this->pc += 2;
    return 3;
}

// ASL d
uint32_t OP_06(NES* this) {
    OP_ASL(this, &this->RAM[VAL1]);
    
    this->pc += 2;
    return 5;
}

// SLO d
uint32_t OP_07(NES* this) {
    uint8_t* value = &this->RAM[VAL1];

    this->p.carry = (*value > 0x7F);
    *value <<= 1;
    this->a |= *value;
    this->p.zero = (this->a == 0);
    this->p.neg  = (this->a > 0x7F);

    this->pc += 2;
    return 5;
}

// PHP
uint32_t OP_08(NES* this) {
    Status p = this->p;

    p.brk = 1;
    p.unused = 1;
    push(this, p.flags);

    this->pc++;
    return 3;
}

// ORA #
uint32_t OP_09(NES* this) {
    OP_ORA(this, VAL1);

    this->pc += 2;
    return 2;
}

// ASL
uint32_t OP_0A(NES* this) {
    this->p.carry = (this->a > 0x7F);
    this->a <<= 1;
    this->p.zero  = (this->a == 0);
    this->p.neg   = (this->a > 0x7F);

    this->pc++;
    return 2;
}

// ORA a
uint32_t OP_0D(NES* this) {
    OP_ORA(this, this->RAM[LE_ADDR]);

    this->pc += 3;
    return 4;
}

// ASL a
uint32_t OP_0E(NES* this) {
    OP_ASL(this, &this->RAM[LE_ADDR]);

    this->pc += 3;
    return 6;
}

// BPL
uint32_t OP_10(NES* this) {
    if (!this->p.neg) {
        this->pc += VAL1 + 2;
        CHECK_PAGE(3);
    } else {
        this->pc += 2;
        return 2;
    }
}

// ORA (d),y
uint32_t OP_11(NES* this) {
    OP_ORA(this, index_dy(this, VAL1));

    this->pc += 2;
    CHECK_PAGE(5);
}

// ORA d,x
uint32_t OP_15(NES* this) {
    OP_ORA(this, index_zx(this, VAL1));

    this->pc += 2;
    return 4;
}

// ASL d,x
uint32_t OP_16(NES* this) {
    OP_ASL(this, &this->RAM[(uint8_t)(VAL1 + this->x)]);

    this->pc += 2;
    return 6;
}

// CLC
uint32_t OP_18(NES* this) {
    this->p.carry = 0;

    this->pc++;
    return 2;
}

// ORA a,y
uint32_t OP_19(NES* this) {
    OP_ORA(this, index_ay(this, LE_ADDR));

    this->pc += 3;
    CHECK_PAGE(4);
}

// ORA a,x
uint32_t OP_1D(NES* this) {
    OP_ORA(this, index_ax(this, LE_ADDR));

    this->pc += 3;
    CHECK_PAGE(4);
}

// ASL a,x
uint32_t OP_1E(NES* this) {
    OP_ASL(this, &this->RAM[LE_ADDR + this->x]);

    this->pc += 3;
    return 7;
}

// JSR
uint32_t OP_20(NES* this) {
    push(this, this->pc);
    push(this, this->pc >> 8);

    this->pc = this->RAM[LE_ADDR];

    return 6;
}

// AND (d,x)
uint32_t OP_21(NES* this) {
    OP_AND(this, this->RAM[VAL1]);
    this->pc += 2;
    return 6;
}

// BIT d
uint32_t OP_24(NES* this) {
    uint8_t res = this->a & this->RAM[VAL1];
    
    this->p.zero = (res == 0);
    this->p.over = ((res & 0x40) == 0x40);
    this->p.neg  = (res > 0x7F);

    this->pc += 2;
    return 3;
}

// AND #
uint32_t OP_25(NES* this) {
    OP_AND(this, this->RAM[VAL1]);
    this->pc += 2;
    return 3;
}

// ROL d
uint32_t OP_26(NES* this) {
    OP_ROL(this, &this->RAM[VAL1]);

    this->pc += 2;
    return 5;
}

// PLP
uint32_t OP_28(NES* this) {
    Status p;
    p.flags = pop(this);

    this->p.carry = p.carry;
    this->p.zero  = p.zero;
    this->p.dec   = p.dec;
    this->p.over  = p.over;
    this->p.neg   = p.neg;

    this->cycleFunc = CyclePLP;
    special_plp = p.intd;

    this->pc++;
    return 4;
}

// AND d
uint32_t OP_29(NES* this) {
    OP_AND(this, VAL1);
    this->pc += 2;
    return 2;
}

// ROL
uint32_t OP_2A(NES* this) {
    uint8_t carry = (this->a > 0x7F);
    this->p.carry = carry;

    this->a <<= 1;
    this->a |= carry;

    this->p.zero  = (this->a == 0);
    this->p.neg   = (this->a > 0x7F);

    this->pc++;
    return 2;
}

// BIT a
uint32_t OP_2C(NES* this) {
    uint8_t res = this->a & this->RAM[LE_ADDR];
    
    this->p.zero = (res == 0);
    this->p.over = ((res & 0x40) == 0x40);
    this->p.neg  = (res > 0x7F);

    this->pc += 3;
    return 4;
}

// AND a
uint32_t OP_2D(NES* this) {
    OP_AND(this, this->RAM[LE_ADDR]);
    this->pc += 3;
    return 4;
}

// ROL a
uint32_t OP_2E(NES* this) {
    OP_ROL(this, &this->RAM[LE_ADDR]);

    this->pc += 3;
    return 6;
}

// BMI
uint32_t OP_30(NES* this) {
    if (this->p.neg) {
        this->pc += VAL1 + 2;
        CHECK_PAGE(3);
    } else {
        this->pc += 2;
        return 2;
    }
}

// AND (d),y
uint32_t OP_31(NES* this) {
    uint16_t addr = index_dy(this, VAL1);
    OP_AND(this, addr);
    this->pc += 2;
    CHECK_PAGE(5);
}

// AND d,x
uint32_t OP_35(NES* this) {
    OP_AND(this, index_zx(this, VAL1));
    this->pc += 2;
    return 4;
}

// ROL d,x
uint32_t OP_36(NES* this) {
    OP_ROL(this, &this->RAM[(uint32_t)(VAL1 + this->x)]);

    this->pc += 2;
    return 6;
}

uint32_t OP_38(NES* this) {
    this->p.carry = 1;
    this->pc++;
    return 2;
}

// AND a,y
uint32_t OP_39(NES* this) {
    uint16_t addr = index_ay(this, LE_ADDR);
    OP_AND(this, addr);
    this->pc += 3;
    CHECK_PAGE(4);
}

// AND a,x
uint32_t OP_3D(NES* this) {
    uint16_t addr = index_ax(this, LE_ADDR);
    OP_AND(this, addr);
    this->pc += 3;
    CHECK_PAGE(4);
}

// ROL a,x
uint32_t OP_3E(NES* this) {
    OP_ROL(this, &this->RAM[LE_ADDR + this->x]);

    this->pc += 3;
    return 7;
}

// RTI
uint32_t OP_40(NES* this) {
    Status p;
    p.flags = pop(this);
    this->p.carry = p.carry;
    this->p.zero  = p.zero;
    this->p.intd  = p.intd;
    this->p.dec   = p.dec;
    this->p.over  = p.over;
    this->p.neg   = p.neg;

    this->pc = (pop(this) << 8) + pop(this);

    return 6;
}

// EOR (d,x)
uint32_t OP_41(NES* this) {
    OP_EOR(this, index_dx(this, VAL1));

    this->pc += 2;
    return 6;
}

// EOR d
uint32_t OP_45(NES* this) {
    OP_EOR(this, this->RAM[VAL1]);

    this->pc += 2;
    return 3;
}

// LSR d
uint32_t OP_46(NES* this) {
    OP_LSR(this, &this->RAM[VAL1]);

    this->pc += 2;
    return 5;
}

// PHA
uint32_t OP_48(NES* this) {
    push(this, this->a);

    this->pc++;
    return 3;
}

// EOR #
uint32_t OP_49(NES* this) {
    OP_EOR(this, VAL1);

    this->pc += 2;
    return 2;
}

// LSR
uint32_t OP_4A(NES* this) {
    this->p.carry = this->a & 1;
    this->a >>= 1;
    this->p.zero  = (this->a == 0);
    this->p.neg   = (this->a > 0x7F);

    this->pc++;
    return 2;
}

// JMP a
uint32_t OP_4C(NES* this) {
    this->pc = this->RAM[LE_ADDR];

    return 3;
}

// EOR a
uint32_t OP_4D(NES* this) {
    OP_EOR(this, LE_ADDR);

    this->pc += 3;
    return 4;
}

// LSR a
uint32_t OP_4E(NES* this) {
    OP_LSR(this, &this->RAM[LE_ADDR]);

    this->pc += 3;
    return 6;
}

// BVC
uint32_t OP_50(NES* this) {
    if (!this->p.over) {
        this->pc += VAL1 + 2;
        CHECK_PAGE(3);
    } else {
        this->pc += 2;
        return 2;
    }
}

// EOR (d),y
uint32_t OP_51(NES* this) {
    OP_EOR(this, index_dy(this, VAL1));

    this->pc += 2;
    CHECK_PAGE(5);
}

// EOR d,x
uint32_t OP_55(NES* this) {
    OP_EOR(this, index_zx(this, VAL1));

    this->pc += 2;
    return 4;
}

// LSR d,x
uint32_t OP_56(NES* this) {
    OP_LSR(this, &this->RAM[(uint8_t)(VAL1 + this->x)]);

    this->pc += 2;
    return 6;
}

// CLI
uint32_t OP_58(NES* this) {
    this->cycleFunc = CyclePLP;
    special_plp = 0;

    this->pc++;
    return 2;
}

// EOR a,y
uint32_t OP_59(NES* this) {
    OP_EOR(this, index_ay(this, LE_ADDR));

    this->pc += 3;
    CHECK_PAGE(4);
}

// EOR a,x
uint32_t OP_5D(NES* this) {
    OP_EOR(this, index_ax(this, LE_ADDR));

    this->pc += 3;
    CHECK_PAGE(4);
}

// LSR a,x
uint32_t OP_5E(NES* this) {
    OP_LSR(this, &this->RAM[LE_ADDR + this->x]);

    this->pc += 3;
    return 7;
}

// RTS
uint32_t OP_60(NES* this) {
    this->pc = (pop(this) << 8) + pop(this);
    this->pc++;
    return 6;
}

// ADC (d,x)
uint32_t OP_61(NES* this) {
    OP_ADC(this, index_dx(this, VAL1));
    this->pc += 2;
    return 6;
}

// ADC d
uint32_t OP_65(NES* this) {
    OP_ADC(this, this->RAM[VAL1]);
    this->pc += 2;
    return 3;
}

// ROR d
uint32_t OP_66(NES* this) {
    OP_ROR(this, &this->RAM[VAL1]);

    this->pc += 2;
    return 5;
}

// PLA
uint32_t OP_68(NES* this) {
    this->a = pop(this);

    this->p.zero = (this->a == 0);
    this->p.neg  = (this->a > 0x7F);

    this->pc++;
    return 4;
}

// ADC #
uint32_t OP_69(NES* this) {
    OP_ADC(this, VAL1);
    this->pc += 2;
    return 2;
}

// ROR
uint32_t OP_6A(NES* this) {
    uint8_t carry = (this->a & 1);
    this->p.carry = carry;

    this->a >>= 1;
    this->a |= carry << 7;

    this->p.zero  = (this->a == 0);
    this->p.neg   = (this->a > 0x7F);

    this->pc++;
    return 2;
}

// JMP i
uint32_t OP_6C(NES* this) {
    this->pc += VAL1;

    return 5;
}

// BVS
uint32_t OP_70(NES* this) {
    if (this->p.over) {
        this->pc += VAL1 + 2;
        CHECK_PAGE(3);
    } else {
        this->pc += 2;
        return 2;
    }
}

// ADC (d),y
uint32_t OP_71(NES* this) {
    uint16_t addr = index_dy(this, LE_ADDR);
    OP_ADC(this, addr);
    this->pc += 2;
    CHECK_PAGE(5);
}

// ADC a
uint32_t OP_6D(NES* this) {
    OP_ADC(this, index_dx(this, VAL1));
    this->pc += 3;
    return 4;
}

// ROR a
uint32_t OP_6E(NES* this) {
    OP_ROR(this, &this->RAM[LE_ADDR]);

    this->pc += 3;
    return 6;
}

// ADC d,x
uint32_t OP_75(NES* this) {
    OP_ADC(this, index_zx(this, VAL1));
    this->pc += 2;
    return 4;
}

// ROR d,x
uint32_t OP_76(NES* this) {
    OP_ROR(this, &this->RAM[(uint8_t)(VAL1 + this->x)]);

    this->pc += 2;
    return 6;
}

// SEI
uint32_t OP_78(NES* this) {
    this->cycleFunc = CyclePLP;
    special_plp = 1;
    this->pc++;
    return 2;
}

// ADC a,y
uint32_t OP_79(NES* this) {
    uint16_t addr = index_ay(this, LE_ADDR);
    OP_ADC(this, addr);
    this->pc += 3;
    
    CHECK_PAGE(4);
}

// ADC a,x
uint32_t OP_7D(NES* this) {
    uint16_t addr = index_ax(this, LE_ADDR);
    OP_ADC(this, addr);
    this->pc += 3;
    
    CHECK_PAGE(4);
}

// ROR a,x
uint32_t OP_7E(NES* this) {
    OP_ROR(this, &this->RAM[LE_ADDR + this->x]);

    this->pc += 3;
    return 7;
}

// STA (d,x)
uint32_t OP_81(NES* this) {
    uint16_t r1 = index_zx(this, VAL1);
    uint16_t r2 = index_zx(this, VAL1 + 1);

    this->RAM[(r1 + r2) * 256] = this->a;
    this->pc += 2;
    return 6;
}

// STY d
uint32_t OP_84(NES* this) {
    this->RAM[VAL1] = this->y;
    this->pc += 2;
    return 3;
}

// STA d
uint32_t OP_85(NES* this) {
    this->RAM[VAL1] = this->a;
    this->pc += 2;
    return 3;
}

// STX d
uint32_t OP_86(NES* this) {
    this->RAM[VAL1] = this->x;
    this->pc += 2;
    return 3;
}

// DEY
uint32_t OP_88(NES* this) {
    this->y--;

    this->p.zero = (this->y == 0);
    this->p.neg  = (this->y > 0x7F);
}

// TXA
uint32_t OP_8A(NES* this) {
    this->a = this->x;

    this->p.zero = (this->a == 0);
    this->p.neg  = (this->a > 0x7F);
    
    this->pc++;
    return 2;
}

// STY a
uint32_t OP_8C(NES* this) {
    this->RAM[LE_ADDR] = this->y;
    this->pc += 3;
    return 4;
}

// STA a
uint32_t OP_8D(NES* this) {
    this->RAM[LE_ADDR] = this->a;
    this->pc += 3;
    return 4;
}

// STX a
uint32_t OP_8E(NES* this) {
    this->RAM[LE_ADDR] = this->x;
    this->pc += 3;
    return 4;
}

// BCC
uint32_t OP_90(NES* this) {
    if (!this->p.carry) {
        this->pc += VAL1 + 2;
        CHECK_PAGE(3);
    } else {
        this->pc += 2;
        return 2;
    }
}

// STA (d,x)
uint32_t OP_91(NES* this) {
    uint16_t r = this->RAM[VAL1] + this->RAM[(VAL1 + 1) % 256] * 256;
    this->RAM[r + this->y] = this->a;
    this->pc += 2;
    return 6;
}

// STY d,x
uint32_t OP_94(NES* this) {
    this->RAM[(uint8_t)(VAL1 + this->x)] = this->y;
    this->pc += 2;
    return 4;
}

// STA d,x
uint32_t OP_95(NES* this) {
    this->RAM[(uint8_t)(VAL1 + this->x)] = this->a;
    this->pc += 2;
    return 4;
}

// STX d,y
uint32_t OP_96(NES* this) {
    this->RAM[(uint8_t)(VAL1 + this->y)] = this->x;
    this->pc += 2;
    return 4;
}

// STA a,y
uint32_t OP_99(NES* this) {
    this->RAM[LE_ADDR + this->y] = this->a;
    this->pc += 3;
    return 5;
}

// TXS
uint32_t OP_9A(NES* this) {
    this->sp = this->x;
    
    this->pc++;
    return 2;
}

// STA a,x
uint32_t OP_9D(NES* this) {
    this->RAM[LE_ADDR + this->x] = this->a;
    this->pc += 3;
    return 5;
}

// LDY #
uint32_t OP_A0(NES* this) {
    OP_LDY(this, VAL1);

    this->pc += 2;
    return 2;
}

// LDA (d,x)
uint32_t OP_A1(NES* this) {
    OP_LDA(this, index_dx(this, VAL1));

    this->pc += 2;
    return 6;
}

// LDX #
uint32_t OP_A2(NES* this) {
    OP_LDX(this, VAL1);

    this->pc += 2;
    return 2;
}

// LDY d
uint32_t OP_A4(NES* this) {
    OP_LDY(this, this->RAM[VAL1]);

    this->pc += 2;
    return 3;
}

// LDA d
uint32_t OP_A5(NES* this) {
    OP_LDA(this, this->RAM[VAL1]);

    this->pc += 2;
    return 3;
}

// LDX d
uint32_t OP_A6(NES* this) {
    OP_LDX(this, this->RAM[VAL1]);

    this->pc += 2;
    return 3;
}

// TAY
uint32_t OP_A8(NES* this) {
    this->y = this->a;

    this->p.zero = (this->y == 0);
    this->p.neg  = (this->y > 0x7F);

    this->pc++;
    return 2;
}

// LDA #
uint32_t OP_A9(NES* this) {
    OP_LDA(this, VAL1);

    this->pc += 2;
    return 2;
}

// TAX
uint32_t OP_AA(NES* this) {
    this->x = this->a;

    this->p.zero = (this->x == 0);
    this->p.neg  = (this->x > 0x7F);
    
    this->pc++;
    return 2;
}

// LDY a
uint32_t OP_AC(NES* this) {
    OP_LDY(this, this->RAM[LE_ADDR]);

    this->pc += 3;
    return 4;
}

// LDA a
uint32_t OP_AD(NES* this) {
    OP_LDA(this, this->RAM[LE_ADDR]);

    this->pc += 3;
    return 4;
}

// LDX a
uint32_t OP_AE(NES* this) {
    OP_LDX(this, this->RAM[LE_ADDR]);

    this->pc += 3;
    return 4;
}

// BCS
uint32_t OP_B0(NES* this) {
    if (this->p.carry) {
        this->pc += VAL1 + 2;
        CHECK_PAGE(3);
    } else {
        this->pc += 2;
        return 2;
    }
}

// LDA (d),y
uint32_t OP_B1(NES* this) {
    OP_LDA(this, index_dy(this, VAL1));

    this->pc += 2;
    CHECK_PAGE(5);
}

// LDY d
uint32_t OP_B4(NES* this) {
    OP_LDY(this, index_zx(this, VAL1));

    this->pc += 2;
    return 4;
}

// LDA d,x
uint32_t OP_B5(NES* this) {
    OP_LDA(this, index_zx(this, VAL1));

    this->pc += 2;
    return 4;
}

// LDX d,y
uint32_t OP_B6(NES* this) {
    OP_LDX(this, index_zy(this, VAL1));

    this->pc += 2;
    return 4;
}

// LDA a,y
uint32_t OP_B9(NES* this) {
    OP_LDA(this, index_ay(this, LE_ADDR));

    this->pc += 3;
    CHECK_PAGE(4);
}

// TSX
uint32_t OP_BA(NES* this) {
    this->x = this->sp;

    this->p.zero = (this->x == 0);
    this->p.neg  = (this->x > 0x7F);
    
    this->pc++;
    return 2;
}

// LDY a
uint32_t OP_BC(NES* this) {
    OP_LDY(this, index_ax(this, LE_ADDR));

    this->pc += 3;
    CHECK_PAGE(4);
}

// LDA a,x
uint32_t OP_BD(NES* this) {
    OP_LDA(this, index_ax(this, LE_ADDR));

    this->pc += 3;
    CHECK_PAGE(4);
}

// LDX a,y
uint32_t OP_BE(NES* this) {
    OP_LDX(this, index_ay(this, LE_ADDR));

    this->pc += 3;
    CHECK_PAGE(4);
}

// CLV
uint32_t OP_B8(NES* this) {
    this->p.over = 0;

    this->pc++;
    return 2;
}

// CPY #
uint32_t OP_C0(NES* this) {
    OP_CPY(this, VAL1);

    this->pc += 2;
    return 2;
}

// CMP (d,x)
uint32_t OP_C1(NES* this) {
    OP_CMP(this, index_dx(this, VAL1));

    this->pc += 2;
    return 6;
}

// CPY d
uint32_t OP_C4(NES* this) {
    OP_CPY(this, this->RAM[VAL1]);

    this->pc += 2;
    return 3;
}

// CMP d
uint32_t OP_C5(NES* this) {
    OP_CMP(this, this->RAM[VAL1]);

    this->pc += 2;
    return 3;
}

// DEC d
uint32_t OP_C6(NES* this) {
    OP_DEC(this, &this->RAM[VAL1]);

    this->pc += 2;
    return 5;
}

// INY
uint32_t OP_C8(NES* this) {
    this->y--;

    this->p.zero = (this->y == 0);
    this->p.neg  = (this->y > 0x7F);
}

// CMP #
uint32_t OP_C9(NES* this) {
    OP_CMP(this, VAL1);

    this->pc++;
    return 2;
}

// DEX
uint32_t OP_CA(NES* this) {
    this->x--;

    this->p.zero = (this->x == 0);
    this->p.neg  = (this->x > 0x7F);

    this->pc++;
    return 2;
}

// CPY a
uint32_t OP_CC(NES* this) {
    OP_CPY(this, this->RAM[LE_ADDR]);

    this->pc += 3;
    return 4;
}

// CMP a
uint32_t OP_CD(NES* this) {
    OP_CMP(this, this->RAM[LE_ADDR]);

    this->pc += 3;
    return 4;
}

// DEC a
uint32_t OP_CE(NES* this) {
    OP_DEC(this, &this->RAM[LE_ADDR]);

    this->pc += 3;
    return 6;
}

// BNE
uint32_t OP_D0(NES* this) {
    if (this->p.zero) {
        this->pc += VAL1 + 2;
        CHECK_PAGE(3);
    } else {
        this->pc += 2;
        return 2;
    }
}

// CMP (d),y
uint32_t OP_D1(NES* this) {
    OP_CMP(this, index_dy(this, VAL1));

    this->pc += 2;
    CHECK_PAGE(5);
}

// CMP d,x
uint32_t OP_D5(NES* this) {
    OP_CMP(this, index_zx(this, VAL1));

    this->pc += 2;
    return 4;
}

// DEC d,x
uint32_t OP_D6(NES* this) {
    OP_DEC(this, &this->RAM[(uint8_t)(this->x + VAL1)]);

    this->pc += 2;
    return 6;
}

// CLD
uint32_t OP_D8(NES* this) {
    this->p.dec = 0;

    this->pc++;
    return 2;
}

// CMP a,x
uint32_t OP_D9(NES* this) {
    OP_CMP(this, index_ay(this, LE_ADDR));

    this->pc += 3;
    CHECK_PAGE(4);
}

// CMP a,x
uint32_t OP_DD(NES* this) {
    OP_CMP(this, index_ax(this, LE_ADDR));

    this->pc += 3;
    CHECK_PAGE(4);
}

// DEC a,x
uint32_t OP_DE(NES* this) {
    OP_DEC(this, &this->RAM[LE_ADDR + this->x]);

    this->pc += 3;
    return 7;
}

// CPX #
uint32_t OP_E0(NES* this) {
    OP_CPX(this, VAL1);

    this->pc += 2;
    return 2;
}

// SBC (d,x)
uint32_t OP_E1(NES* this) {
    OP_SBC(this, index_dx(this, VAL1));
    this->pc += 2;
    return 6;
}

// CPX d
uint32_t OP_E4(NES* this) {
    OP_CPX(this, this->RAM[VAL1]);

    this->pc += 2;
    return 3;
}

// SBC d
uint32_t OP_E5(NES* this) {
    OP_SBC(this, this->RAM[VAL1]);
    this->pc += 2;
    return 3;
}

// INC d
uint32_t OP_E6(NES* this) {
    OP_DEC(this, &this->RAM[VAL1]);

    this->pc += 2;
    return 5;
}

// INX
uint32_t OP_E8(NES* this) {
    this->x--;

    this->p.zero = (this->x == 0);
    this->p.neg  = (this->x > 0x7F);
}

// SBC #
uint32_t OP_E9(NES* this) {
    OP_SBC(this, VAL1);
    this->pc += 2;
    return 2;
}

// NOP
uint32_t OP_EA(NES* this) {
    this->pc++;
    return 2;
}

// CPX a
uint32_t OP_EC(NES* this) {
    OP_CPX(this, this->RAM[LE_ADDR]);

    this->pc += 3;
    return 4;
}

// SBC a
uint32_t OP_ED(NES* this) {
    OP_SBC(this, this->RAM[LE_ADDR]);
    this->pc += 3;
    return 4;
}

// INC a
uint32_t OP_EE(NES* this) {
    OP_DEC(this, &this->RAM[LE_ADDR]);

    this->pc += 3;
    return 6;
}

// BEQ
uint32_t OP_F0(NES* this) {
    if (this->p.zero) {
        this->pc += VAL1 + 2;
        CHECK_PAGE(3);
    } else {
        this->pc += 2;
        return 2;
    }
}

// SBC (d),y
uint32_t OP_F1(NES* this) {
    OP_SBC(this, index_dy(this, VAL1));
    this->pc += 2;
    CHECK_PAGE(5);
}

// SBC d,x
uint32_t OP_F5(NES* this) {
    OP_SBC(this, index_zx(this, VAL1));
    this->pc += 2;
    return 4;
}

// INC d
uint32_t OP_F6(NES* this) {
    OP_DEC(this, &this->RAM[(uint8_t)(VAL1 + this->x)]);

    this->pc += 2;
    return 6;
}

uint32_t OP_F8(NES* this) {
    this->p.dec = 1;
    this->pc++;
    return 2;
}

// SBC a,y
uint32_t OP_F9(NES* this) {
    OP_SBC(this, index_ay(this, LE_ADDR));
    this->pc += 3;
    CHECK_PAGE(4);
}

// SBC a,x
uint32_t OP_FD(NES* this) {
    OP_SBC(this, index_ax(this, LE_ADDR));
    this->pc += 3;
    CHECK_PAGE(4);
}

// INC a,x
uint32_t OP_FE(NES* this) {
    OP_DEC(this, &this->RAM[LE_ADDR + this->x]);

    this->pc += 3;
    return 7;
}