#include "opcodes.h"

// opcodes x2 aside from 0x82, 0xA2, 0xC2, and 0xE2
void OP_STP(NES* this) {
    return -1;
}

// BRK
void OP_00(NES* this) {
    uint8_t v1 = getVal(this);

    push(this, this->pc);
    push(this, this->pc >> 8);

    Status p = this->p;
    p.unused = 1;
    p.brk = 1;
    push(this, p.flags);
    this->p.intd = 1;

    this->pc = read(this, 0xFFFE) + (read(this, 0xFFFF) << 8);
}

// ORA (d,x)
void OP_01(NES* this) {
    uint8_t val, mem;
    val = getVal(this);
    mem = index_dx(this, val, NULL);
    OP_ORA(this, mem);
}

// SLO (d,x)
void OP_03(NES* this) {
    uint8_t val, mem;
    uint16_t addr;
    val = getVal(this);
    mem = index_dx(this, val, &addr);
    write(this, addr, mem);
    OP_SLO(this, &mem);
    write(this, addr, mem);
}

// NOP d
void OP_04(NES* this) {
    read(this, getVal(this));
}

// ORA d
void OP_05(NES* this) {
    uint8_t val = getVal(this);
    uint8_t mem = read(this, val);
    OP_ORA(this, mem);
}

// ASL d
void OP_06(NES* this) {
    uint8_t val = getVal(this);
    uint8_t mem = read(this, val);
    write(this, val, mem);
    OP_ORA(this, mem);
    write(this, val, mem);
}

// SLO d
void OP_07(NES* this) {
    uint8_t val = getVal(this);
    uint8_t mem = read(this, val);
    write(this, val, mem);
    OP_SLO(this, &mem);
    write(this, val, mem);
}

// PHP
void OP_08(NES* this) {
    Status p;

    readVal(this);
    p = this->p;
    p.brk = 1;
    p.unused = 1;
    push(this, p.flags);
}

// ORA #
void OP_09(NES* this) {
    OP_ORA(this, getVal(this));
}

// ASL
void OP_0A(NES* this) {
    readVal(this);

    this->p.carry = (this->a > 0x7F);
    this->a <<= 1;
    this->p.zero  = (this->a == 0);
    this->p.neg   = (this->a > 0x7F);
}

// ANC #
void OP_0B(NES* this) {
    uint8_t val = getVal(this);
    OP_AND(this, val);
    this->p.carry = this->p.neg;
}

// NOP a
void OP_0C(NES* this) {
    uint8_t val1 = getVal(this);
    uint8_t val2 = getVal(this);
    read(this, val1 + (val2 << 8));
}

// ORA a
void OP_0D(NES* this) {
    uint8_t val1 = getVal(this);
    uint8_t val2 = getVal(this);
    uint16_t mem = read(this, val1 + (val2 << 8));
    OP_ORA(this, mem);
}

// ASL a
void OP_0E(NES* this) {
    uint8_t val1 = getVal(this);
    uint8_t val2 = getVal(this);
    uint16_t addr = val1 + (val2 << 8);
    uint8_t mem = read(this, addr);
    write(this, addr, mem);
    OP_ASL(this, &mem);
    write(this, addr, mem);
}

// SLO a
void OP_0F(NES* this) {
    uint8_t val1 = getVal(this);
    uint8_t val2 = getVal(this);
    uint16_t addr = val1 + (val2 << 8);
    uint8_t mem = read(this, addr);
    write(this, addr, mem);
    OP_SLO(this, &mem);
    write(this, addr, mem);
}

// BPL
void OP_10(NES* this) {
    int8_t val = getVal(this);
    if (this->p.neg) return;
    uint16_t branch = (uint8_t)(this->pc + val) + this->pc & 0xF0;
    getVal(this);
    if (branch == this->pc + val) {
        this->pc = branch;
        return;
    }
    getVal(this);
    this->pc = this->pc + val;
}

// ORA (d),y
void OP_11(NES* this) {
    uint8_t val = getVal(this);
    uint8_t mem = index_dy(this, val, NULL);
    OP_ORA(this, mem);
}

// SLO (d),y
void OP_13(NES* this) {
    uint8_t val = getVal(this);
    uint16_t addr;
    uint8_t mem = index_dy(this, val, &addr);
    write(this, addr, mem);
    OP_SLO(this, &mem);
    write(this, addr, mem);
}

// NOP d,x
void OP_14(NES* this) {
    uint8_t val = getVal(this);
    read(this, val);
    index_zx(this, val, NULL);
}

// ORA d,x
void OP_15(NES* this) {
    uint8_t val = getVal(this);
    read(this, val);
    val = index_zx(this, val, NULL);
    OP_ORA(this, val);
}

// ASL d,x
void OP_16(NES* this) {
    uint8_t val = getVal(this);
    uint16_t addr;
    read(this, val);
    val = index_zx(this, val, &addr);
    write(this, addr, val);
    OP_ASL(this, &val);
    write(this, addr, val);
}

// SLO d,x
void OP_17(NES* this) {
    uint8_t val = getVal(this);
    uint16_t addr;
    read(this, val);
    val = index_zx(this, val, &addr);
    write(this, addr, val);
    OP_SLO(this, &val);
    write(this, addr, val);
}

// CLC
void OP_18(NES* this) {
    readVal(this);
    this->p.carry = 0;
}

// ORA a,y
void OP_19(NES* this) {
    uint16_t val = getVal(this) + (getVal(this) << 8) + this->y;
    uint8_t mem = index_ay(this, val, NULL);
    OP_ORA(this, mem);
}

// NOP
void OP_1A(NES* this) {
    readVal(this);
}

// SLO a,y
void OP_1B(NES* this) {
    uint16_t val = getVal(this) + (getVal(this) << 8) + this->y;
    uint16_t addr;
    uint8_t mem = index_ay(this, val, &addr);
    write(this, addr, mem);
    OP_SLO(this, &mem);
    write(this, addr, mem);
}

// NOP a,x
void OP_1C(NES* this) {
    uint16_t val = getVal(this) + (getVal(this) << 8) + this->y;
    index_ax(this, val, NULL);
}

// ORA a,x
void OP_1D(NES* this) {
    uint16_t val = getVal(this) + (getVal(this) << 8) + this->y;
    uint8_t mem = index_ax(this, val, NULL);
    OP_ORA(this, mem);
}

// ASL a,x
void OP_1E(NES* this) {
    uint16_t val = getVal(this) + (getVal(this) << 8) + this->y;
    uint8_t mem = index_ax(this, val, NULL);
    write(this, val, mem);
    OP_ASL(this, &mem);
    write(this, val, mem);
}

// SLO a,x
void OP_1F(NES* this) {
    uint16_t val = getVal(this) + (getVal(this) << 8) + this->y;
    uint16_t addr;
    uint8_t mem = index_ax(this, val, &addr);
    write(this, addr, mem);
    OP_SLO(this, &mem);
    write(this, addr, mem);
}

// JSR a
void OP_20(NES* this) {
    uint8_t addr_lo = getVal(this);
    ntrnlop(this);
    push(this, this->pc + 2);
    push(this, this->pc >> 8);
    this->pc = addr_lo + (getVal(this) << 8);
}

// AND (d,x)
void OP_21(NES* this) {
    uint8_t val = getVal(this);
    uint8_t mem = index_dx(this, val, NULL);
    OP_AND(this, mem);
}

// RLA (d,x)
void OP_23(NES* this) {
    uint8_t val = getVal(this);
    uint16_t addr;
    uint8_t mem = index_dx(this, val, &addr);
    write(this, addr, val);
    OP_RLA(this, &mem);
    write(this, addr, val);
}

// BIT d
void OP_24(NES* this) {
    uint8_t val = getVal(this);
    uint8_t res = this->a & read(this, val);
    
    this->p.zero = (res == 0);
    this->p.over = ((res & 0x40) == 0x40);
    this->p.neg  = (res > 0x7F);
}

// AND d
void OP_25(NES* this) {
    uint8_t val = getVal(this);
    uint8_t mem = read(this, val);
    OP_AND(this, mem);
}

// ROL d
void OP_26(NES* this) {
    uint8_t val = getVal(this);
    uint8_t mem = read(this, val);
    write(this, val, mem);
    OP_ROL(this, mem);
    write(this, val, mem);
}

// RLA d
void OP_27(NES* this) {
    uint8_t val = getVal(this);
    uint8_t mem = read(this, val);
    write(this, val, mem);
    OP_RLA(this, mem);
    write(this, val, mem);
}

// PLP
void OP_28(NES* this) {
    readVal(this);
    pop(this, NULL);

    Status p;
    p.flags = pop(this, NULL);
    this->p.carry = p.carry;
    this->p.zero  = p.zero;
    this->p.dec   = p.dec;
    this->p.over  = p.over;
    this->p.neg   = p.neg;

    this->cycleFunc = CyclePLP;
    special_plp = p.intd;
}

// AND #
void OP_29(NES* this) {
    OP_AND(this, getVal(this));
}

// ROL
void OP_2A(NES* this) {
    readVal(this);
    uint8_t carry = (this->a > 0x7F);
    this->p.carry = carry;

    this->a <<= 1;
    this->a |= carry;

    this->p.zero  = (this->a == 0);
    this->p.neg   = (this->a > 0x7F);
}

// ANC #
void OP_2B(NES* this) {
    OP_AND(this, getVal(this));
    this->p.carry = this->p.neg;
}

// BIT a
void OP_2C(NES* this) {
    uint16_t addr = getVal(this) + (getVal(this) << 8);
    uint8_t res = this->a & read(this, addr);
    
    this->p.zero = (res == 0);
    this->p.over = ((res & 0x40) == 0x40);
    this->p.neg  = (res > 0x7F);
}

// AND a
void OP_2D(NES* this) {
    uint16_t addr = getVal(this) + (getVal(this) << 8);
    OP_AND(this, read(this, addr));
}

// ROL a
void OP_2E(NES* this) {
    uint16_t addr = getVal(this) + (getVal(this) << 8);
    uint8_t mem = read(this, addr);
    write(this, addr, mem);
    OP_ROL(this, &mem);
    write(this, addr, mem);
}

// RLA a
void OP_2F(NES* this) {
    uint16_t addr = getVal(this) + (getVal(this) << 8);
    uint8_t mem = read(this, addr);
    write(this, addr, mem);
    OP_RLA(this, &mem);
    write(this, addr, mem);
}

// BMI
void OP_30(NES* this) {
    int8_t val = getVal(this);
    if (!this->p.neg) return;
    uint16_t branch = (uint8_t)(this->pc + val) + this->pc & 0xF0;
    getVal(this);
    if (branch == this->pc + val) {
        this->pc = branch;
        return;
    }
    getVal(this);
    this->pc = this->pc + val;
}

// AND (d),y
void OP_31(NES* this) {
    uint8_t val = getVal(this);
    uint8_t mem = index_dy(this, val, NULL);
    OP_AND(this, mem);
}

// RLA (d),y
void OP_33(NES* this) {
    uint8_t val = getVal(this);
    uint16_t addr;
    uint8_t mem = index_dy(this, val, addr);
    write(this, addr, mem);
    OP_RLA(this, &mem);
    write(this, addr, mem);
}

// NOP d,x
void OP_34(NES* this) {
    uint8_t val = getVal(this);
    read(this, val);
    index_zx(this, val, NULL);
}

// AND d,x
void OP_35(NES* this) {
    uint8_t val = getVal(this);
    read(this, val);
    OP_AND(this, index_zx(this, val, NULL));
}

// ROL d,x
void OP_36(NES* this) {
    uint8_t val = getVal(this);
    uint16_t addr;
    read(this, val);
    uint8_t mem = index_zx(this, val, &addr);
    write(this, addr, mem);
    OP_ROL(this, &mem);
    write(this, addr, mem);
}

// RLA d,x
void OP_37(NES* this) {
    uint8_t val = getVal(this);
    uint16_t addr;
    read(this, val);
    uint8_t mem = index_zx(this, val, &addr);
    write(this, addr, mem);
    OP_RLA(this, &mem);
    write(this, addr, mem);
}

// SEC
void OP_38(NES* this) {
    readVal(this);
    this->p.carry = 1;
}

// AND a,y
void OP_39(NES* this) {
    uint8_t val1 = getVal(this);
    uint8_t val2 = getVal(this);
    uint8_t mem = index_ay(this, val1 + (val2 << 8), NULL);
    OP_AND(this, mem);
}

// NOP
void OP_3A(NES* this) {
    readVal(this);
}

// RLA a,y
void OP_3B(NES* this) {
    uint8_t val1 = getVal(this);
    uint8_t val2 = getVal(this);
    uint16_t addr;
    uint8_t mem = index_ay(this, val1 + (val2 << 8), &addr);
    write(this, addr, mem);
    OP_RLA(this, &mem);
    write(this, addr, mem);
}

// NOP a,x
void OP_3C(NES* this) {
    uint8_t val1 = getVal(this);
    uint8_t val2 = getVal(this);
    uint8_t mem = index_ax(this, val1 + (val2 << 8), NULL);
}

// AND a,x
void OP_3D(NES* this) {
    uint8_t val1 = getVal(this);
    uint8_t val2 = getVal(this);
    uint8_t mem = index_ax(this, val1 + (val2 << 8), NULL);
    OP_AND(this, mem);
}

// ROL a,x
void OP_3E(NES* this) {
    uint8_t val1 = getVal(this);
    uint8_t val2 = getVal(this);
    uint16_t addr;
    uint8_t mem = index_ax(this, val1 + (val2 << 8), &addr);
    write(this, addr, mem);
    OP_ROL(this, &mem);
    write(this, addr, mem);
}

// RLA a,x
void OP_3F(NES* this) {
    uint8_t val1 = getVal(this);
    uint8_t val2 = getVal(this);
    uint16_t addr;
    uint8_t mem = index_ax(this, val1 + (val2 << 8), &addr);
    write(this, addr, mem);
    OP_RLA(this, &mem);
    write(this, addr, mem);
}

// RTI
void OP_40(NES* this) {
    Status p;
    readVal(this);
    pop(this, NULL);
    p.flags = pop(this, NULL);
    this->p.carry = p.carry;
    this->p.zero  = p.zero;
    this->p.intd  = p.intd;
    this->p.dec   = p.dec;
    this->p.over  = p.over;
    this->p.neg   = p.neg;
    this->pc = (pop(this, NULL) << 8) + pop(this, NULL);
}

// EOR (d,x)
void OP_41(NES* this) {
    uint8_t val = getVal(this);
    uint8_t mem = index_dx(this, val, NULL);
    OP_EOR(this, mem);
}

// SRE (d,x)
void OP_43(NES* this) {
    uint8_t val = getVal(this);
    uint16_t addr;
    uint8_t mem = index_dx(this, val, &addr);
    write(this, addr, mem);
    OP_SRE(this, mem);
    write(this, addr, mem);
}

// NOP d
void OP_44(NES* this) {
    read(this, getVal(this));
}

// EOR d
void OP_45(NES* this) {
    OP_EOR(this, read(this, getVal(this)));
}

// LSR d
void OP_46(NES* this) {
    uint8_t val = getVal(this);
    uint8_t mem = read(this, val);
    write(this, val, mem);
    OP_LSR(this, &mem);
    write(this, val, mem);
}

// SRE d
void OP_47(NES* this) {
    uint8_t val = getVal(this);
    uint8_t mem = read(this, val);
    write(this, val, mem);
    OP_SRE(this, &mem);
    write(this, val, mem);
}

// PHA
void OP_48(NES* this) {
    readVal(this);
    push(this, this->a);
}

// EOR #
void OP_49(NES* this) {
    OP_EOR(this, getVal(this));
}

// LSR
void OP_4A(NES* this) {
    readVal(this);
    this->p.carry = this->a & 1;
    this->a >>= 1;
    this->p.zero  = (this->a == 0);
    this->p.neg   = (this->a > 0x7F);
}

// ALR #
void OP_4B(NES* this) {
    this->p.carry = this->a & 1;

    this->a &= getVal(this);
    this->a >>= 1;

    this->p.zero  = (this->a == 0);
    this->p.neg   = (this->a > 0x7F);

    this->pc += 2;
}

// JMP a
void OP_4C(NES* this) {
    this->pc = getVal(this) + (getVal(this) << 8);
}

// EOR a
void OP_4D(NES* this) {
    uint16_t addr = getVal(this) + (getVal(this) << 8);
    OP_EOR(this, read(this, addr));
}

// LSR a
void OP_4E(NES* this) {
    uint16_t addr = getVal(this) + (getVal(this) << 8);
    uint8_t mem = read(this, addr);
    write(this, addr, mem);
    OP_LSR(this, &mem);
    write(this, addr, mem);
}

// SRE a
void OP_4F(NES* this) {
    uint16_t addr = getVal(this) + (getVal(this) << 8);
    uint8_t mem = read(this, addr);
    write(this, addr, mem);
    OP_SRE(this, &mem);
    write(this, addr, mem);
}

// BVC
void OP_50(NES* this) {
    int8_t val = getVal(this);
    if (this->p.over) return;
    uint16_t branch = (uint8_t)(this->pc + val) + this->pc & 0xF0;
    getVal(this);
    if (branch == this->pc + val) {
        this->pc = branch;
        return;
    }
    getVal(this);
    this->pc = this->pc + val;
}

// EOR (d),y
void OP_51(NES* this) {
    uint8_t val = getVal(this);
    uint8_t mem = index_dy(this, val, NULL);
    OP_EOR(this, mem);
}

// SRE (d),y
void OP_53(NES* this) {
    uint8_t val = getVal(this);
    uint16_t addr;
    uint8_t mem = index_dy(this, val, &addr);
    write(this, addr, mem);
    OP_SRE(this, &mem);
    write(this, addr, mem);
}

// NOP d,x
void OP_54(NES* this) {
    uint8_t val = getVal(this);
    read(this, val);
    index_zx(this, val, NULL);
}

// EOR d,x
void OP_55(NES* this) {
    uint8_t val = getVal(this);
    read(this, val);
    OP_EOR(this, index_zx(this, val, NULL));
}

// LSR d,x
void OP_56(NES* this) {
    uint8_t val = getVal(this);
    uint16_t addr;
    read(this, val);
    uint8_t mem = index_zx(this, val, &addr);
    write(this, addr, mem);
    OP_LSR(this, &mem);
    write(this, addr, mem);
}

// SRE d,x
void OP_57(NES* this) {
    uint8_t val = getVal(this);
    uint16_t addr;
    read(this, val);
    uint8_t mem = index_zx(this, val, &addr);
    write(this, addr, mem);
    OP_SRE(this, &mem);
    write(this, addr, mem);
}


// CLI
void OP_58(NES* this) {
    readVal(this);
    this->cycleFunc = CyclePLP;
    special_plp = 0;
}

// EOR a,y
void OP_59(NES* this) {
    uint16_t addr = getVal(this) + (getVal(this) << 8);
    OP_EOR(this, index_ay(this, addr, NULL));
}

// NOP
void OP_5A(NES* this) {
    readVal(this);
}

// SRE a,y
void OP_5B(NES* this) {
    uint16_t addr = getVal(this) + (getVal(this) << 8);
    uint8_t mem = index_ay(this, addr, &addr);
    write(this, addr, mem);
    OP_SRE(this, &mem);
    write(this, addr, mem);
}

// IGN a,x
void OP_5C(NES* this) {
    uint16_t addr = getVal(this) + (getVal(this) << 8);
    index_ax(this, addr, NULL);
}

// EOR a,x
void OP_5D(NES* this) {
    uint16_t addr = getVal(this) + (getVal(this) << 8);
    OP_EOR(this, index_ax(this, addr, NULL));
}

// LSR a,x
void OP_5E(NES* this) {
    uint16_t addr = getVal(this) + (getVal(this) << 8);
    uint8_t mem = index_ax(this, addr, &addr);
    write(this, addr, mem);
    OP_LSR(this, &mem);
    write(this, addr, mem);
}

// SRE a,x
void OP_5F(NES* this) {
    uint16_t addr = getVal(this) + (getVal(this) << 8);
    uint8_t mem = index_ax(this, addr, &addr);
    write(this, addr, mem);
    OP_SRE(this, &mem);
    write(this, addr, mem);
}

// RTS
void OP_60(NES* this) {
    VAL1;
    pop(this);
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
    read(this, getVal(this));
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
    pop(this);
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
    OP_ADC(this, *read(this, LE_ADDR));
    this->pc += 3;
}

// ROR a
void OP_6E(NES* this) {
    uint16_t addr = LE_ADDR;
    uint8_t* mem = read(this, addr);
    write(this, addr, *mem);

    OP_ROR(this, mem);
    write(this, addr, *mem);

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
    uint8_t val = getVal(this);
    read(this, val);
    index_zx(this, val, NULL);
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
    VAL1;
    this->cycleFunc = CyclePLP;
    special_plp = 1;
    this->pc++;
}

// ADC a,y
void OP_79(NES* this) {
    OP_ADC(this, *index_ay(this, LE_ADDR));
    this->pc += 3;
}

// NOP
void OP_7A(NES* this) {
    readVal(this);
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
    OP_ADC(this, *index_ax(this, LE_ADDR));
    this->pc += 3;
}

// ROR a,x
void OP_7E(NES* this) {
    uint16_t addr = LE_ADDR;
    uint8_t* mem = index_ax(this, addr);
    write(this, addr, *mem);

    OP_ROR(this, mem);
    write(this, addr, *mem);

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
    VAL1;
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
    VAL1;
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

// TYA
void OP_98(NES* this) {
    VAL1;
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
    VAL1;
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
    VAL1;
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
    VAL1;
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
    VAL1;
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
    VAL1;
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
    uint8_t val = VAL1;
    uint8_t mem = *read(this, val);

    OP_CPY(this, mem);
    write(this, val, mem);
    this->pc += 2;
}

// CMP d
void OP_C5(NES* this) {
    uint8_t val = VAL1;
    uint8_t mem = *read(this, val);
    
    OP_CMP(this, mem);
    write(this, val, mem);
    this->pc += 2;
}

// DEC d
void OP_C6(NES* this) {
    uint8_t val = VAL1;
    uint8_t* mem = read(this, val);
    
    OP_DEC(this, mem);
    write(this, val, *mem);
    this->pc += 2;
}

// DCP d
void OP_C7(NES* this) {
    OP_DCP(this, read(this, VAL1));
    this->pc += 2;
}

// INY
void OP_C8(NES* this) {
    VAL1;
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
    VAL1;
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
    uint16_t addr = LE_ADDR;
    uint8_t* mem = read(this, addr);
    write(this, addr, *mem);

    OP_DEC(this, mem);
    write(this, addr, *mem);

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
    uint8_t val = getVal(this);
    read(this, val);
    index_zx(this, val, NULL);
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
    VAL1;
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
    readVal(this);
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
    uint16_t addr = LE_ADDR;
    uint8_t* mem = index_ax(this, addr);
    write(this, addr, *mem);

    OP_DEC(this, mem);
    write(this, addr, *mem);

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
    VAL1;
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
    readVal(this);
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
    uint16_t addr = LE_ADDR;
    uint8_t* mem = read(this, addr);
    write(this, addr, *mem);

    OP_INC(this, mem);
    write(this, addr, *mem);
    
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
    uint8_t val = getVal(this);
    read(this, val);
    index_zx(this, val, NULL);
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

// SED
void OP_F8(NES* this) {
    VAL1;
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
    readVal(this);
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
    uint16_t addr = LE_ADDR;
    uint8_t* mem = index_ax(this, addr);
    write(this, addr, *mem);

    OP_INC(this, mem);
    write(this, addr, *mem);

    this->pc += 3;
}

// ISC a,x
void OP_FF(NES* this) {
    OP_ISC(this, index_ax(this, LE_ADDR));
    this->pc += 3;
}