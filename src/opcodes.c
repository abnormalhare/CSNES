#include "opcodes.h"

uint32_t page_crossed = 0;

void push(NES* this, uint8_t byte) {
    this->RAM[this->sp--] = byte;
}

uint8_t pop(NES* this) {
    return this->RAM[++this->sp];
}

uint8_t index_zx(NES* this, uint8_t byte) {
    return this->RAM[(this->x + byte) % 256];
}

uint8_t index_zy(NES* this, uint8_t byte) {
    return this->RAM[(this->y + byte) % 256];
}

uint8_t index_ax(NES* this, uint16_t addr) {
    if ((addr % 256) + this->x > 0xFF) {
        page_crossed = 1;
    } else {
        page_crossed = 0;
    }
    return this->RAM[this->x + addr];
}

uint8_t index_ay(NES* this, uint16_t addr) {
    if ((addr % 256) + this->y > 0xFF) {
        page_crossed = 1;
    } else {
        page_crossed = 0;
    }
    return this->RAM[this->y + addr];
}

uint8_t index_dx(NES* this, uint8_t ind) {
    uint16_t r1 = index_zx(this, ind);
    uint16_t r2 = index_zx(this, ind + 1);

    return this->RAM[(r1 + r2) * 256];
}

uint8_t index_dy(NES* this, uint8_t ind) {
    uint16_t r = this->RAM[ind] + this->RAM[(ind + 1) % 256] * 256;
    if ((r % 256) + this->y > 0xFF) {
        page_crossed = 1;
    } else {
        page_crossed = 0;
    }
    return this->RAM[r + this->y];
}

uint32_t OP_STP(NES* this) {
    return -1;
}

void OP_ADC(NES* this, uint16_t mem) {
    uint16_t value = this->a + mem + this->p.carry;
    this->a = value;

    this->p.carry = (value > 0xFF);
    this->p.zero  = (value == 0);
    this->p.over  = (((value ^ this->a) & (value ^ mem) & 0x80) == 0x80);
    this->p.neg   = (this->a > 0x7F);
}

void OP_AND(NES* this, uint8_t mem) {
    this->a &= mem;

    this->p.zero = (this->a == 0);
    this->p.neg  = (this->a > 0x7F);
}

void OP_CMP(NES* this, uint8_t mem) {
    int8_t val = this->a - mem;

    this->p.carry = (val >= 0);
    this->p.zero  = (val == 0);
    this->p.neg   = (val < 0);
}

void OP_CPX(NES* this, uint8_t mem) {
    int8_t val = this->x - mem;

    this->p.carry = (val >= 0);
    this->p.zero  = (val == 0);
    this->p.neg   = (val < 0);
}

void OP_CPY(NES* this, uint8_t mem) {
    int8_t val = this->y - mem;

    this->p.carry = (val >= 0);
    this->p.zero  = (val == 0);
    this->p.neg   = (val < 0);
}

void OP_DEC(NES* this, uint8_t* mem) {
    *mem--;

    this->p.zero = (*mem == 0);
    this->p.neg  = (*mem > 127);
}

void OP_EOR(NES* this, uint8_t mem) {
    this->a ^= VAL1;

    this->p.zero = (this->a == 0);
    this->p.neg  = (this->a > 127);
}

// BRK
uint32_t OP_00(NES* this) {
    push(this, this->pc);
    push(this, (this->pc & 0xFF00) >> 4);

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
    this->a |= index_dx(this, VAL1);

    this->p.zero = (this->a == 0);
    this->p.neg  = (this->a > 0x7F);

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
    this->a |= this->RAM[VAL1];

    this->p.zero = (this->a == 0);
    this->p.neg  = (this->a > 0x7F);

    this->pc += 2;
    return 3;
}

// ASL d
uint32_t OP_06(NES* this) {
    uint8_t* value = &this->RAM[VAL1];

    this->p.carry = (*value > 0x7F);
    *value <<= 1;
    this->p.zero  = (*value == 0);
    this->p.neg   = (*value > 0x7F);
    
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

// ASL
uint32_t OP_0A(NES* this) {
    this->p.carry = (this->a > 0x7F);
    this->a <<= 1;
    this->p.zero  = (this->a == 0);
    this->p.neg   = (this->a > 0x7F);

    this->pc++;
    return 2;
}

// ASL a
uint32_t OP_0E(NES* this) {
    uint8_t* mem = &this->RAM[(VAL1 << 4) + VAL2];
    this->p.carry = (*mem > 0x7F);
    *mem <<= 1;
    this->p.zero  = (*mem == 0);
    this->p.neg   = (*mem > 0x7F);

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

// ASL d,x
uint32_t OP_16(NES* this) {
    uint8_t* mem = &this->RAM[(uint8_t)(this->x + VAL1)];
    this->p.carry = (*mem > 0x7F);
    *mem <<= 1;
    this->p.zero  = (*mem == 0);
    this->p.neg   = (*mem > 0x7F);

    this->pc += 2;
    return 6;
}

// CLC
uint32_t OP_18(NES* this) {
    this->p.carry = 0;

    this->pc++;
    return 2;
}

// ASL a,x
uint32_t OP_1E(NES* this) {
    uint8_t* mem = &this->RAM[(VAL1 << 4) + VAL2 + this->x];
    this->p.carry = (*mem > 0x7F);
    *mem <<= 1;
    this->p.zero  = (*mem == 0);
    this->p.neg   = (*mem > 0x7F);

    this->pc += 3;
    return 7;
}

// AND (d,x)
uint32_t OP_21(NES* this) {
    OP_AND(this, index_dx(this, VAL1));
    this->pc += 2;
    return 6;
}

// BIT d
uint32_t OP_24(NES* this) {
    uint8_t res = this->a & this->RAM[VAL1];
    
    this->p.zero = (res == 0);
    this->p.over = ((res & 0x40) == 0x40);
    this->p.neg  = (res > 127);

    this->pc += 2;
    return 3;
}

// AND #
uint32_t OP_25(NES* this) {
    OP_AND(this, this->RAM[VAL1]);
    this->pc += 2;
    return 3;
}

// AND d
uint32_t OP_29(NES* this) {
    OP_AND(this, VAL1);
    this->pc += 2;
    return 2;
}

// BIT a
uint32_t OP_2C(NES* this) {
    uint8_t res = this->a & this->RAM[(VAL1 << 4) + VAL2];
    
    this->p.zero = (res == 0);
    this->p.over = ((res & 0x40) == 0x40);
    this->p.neg  = (res > 127);

    this->pc += 3;
    return 4;
}

// AND a
uint32_t OP_2D(NES* this) {
    OP_AND(this, this->RAM[(VAL1 << 4) + VAL2]);
    this->pc += 3;
    return 4;
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

// AND a,y
uint32_t OP_3D(NES* this) {
    uint16_t addr = index_ay(this, (VAL1 << 4) + VAL2);
    OP_AND(this, addr);
    this->pc += 3;
    CHECK_PAGE(4);
}

// AND a,x
uint32_t OP_3D(NES* this) {
    uint16_t addr = index_ax(this, (VAL1 << 4) + VAL2);
    OP_AND(this, addr);
    this->pc += 3;
    CHECK_PAGE(4);
}

// EOR (d,x)
uint32_t OP_41(NES* this) {
    OP_EOR(this, index_dx(this, VAL1));

    this->pc += 2;
    return 6;
}

// EOR d
uint32_t OP_49(NES* this) {
    OP_EOR(this, this->RAM[VAL1]);

    this->pc += 2;
    return 3;
}

// EOR #
uint32_t OP_49(NES* this) {
    OP_EOR(this, VAL1);

    this->pc += 2;
    return 2;
}

// EOR a
uint32_t OP_4D(NES* this) {
    OP_EOR(this, (VAL1 << 4) + VAL2);

    this->pc += 3;
    return 4;
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

// CLI
uint32_t OP_58(NES* this) {
    this->p.intd = 0;

    this->pc++;
    return 2;
}

// EOR a,y
uint32_t OP_59(NES* this) {
    OP_EOR(this, index_ay(this, (VAL1 << 4) + VAL2));

    this->pc += 3;
    CHECK_PAGE(4);
}

// EOR a,x
uint32_t OP_5D(NES* this) {
    OP_EOR(this, index_ax(this, (VAL1 << 4) + VAL2));

    this->pc += 3;
    CHECK_PAGE(4);
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

// ADC #
uint32_t OP_69(NES* this) {
    OP_ADC(this, VAL1);
    this->pc += 2;
    return 2;
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
    uint16_t addr = index_dy(this, (VAL1 << 4) + VAL2);
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

// ADC d,x
uint32_t OP_75(NES* this) {
    OP_ADC(this, index_zx(this, VAL1));
    this->pc += 2;
    return 4;
}

// ADC a,y
uint32_t OP_79(NES* this) {
    uint16_t addr = index_ay(this, (VAL1 << 4) + VAL2);
    OP_ADC(this, addr);
    this->pc += 3;
    
    CHECK_PAGE(4);
}

// ADC a,x
uint32_t OP_7D(NES* this) {
    uint16_t addr = index_ax(this, (VAL1 << 4) + VAL2);
    OP_ADC(this, addr);
    this->pc += 3;
    
    CHECK_PAGE(4);
}

// DEY
uint32_t OP_88(NES* this) {
    this->y--;

    this->p.zero = (this->y == 0);
    this->p.neg  = (this->y > 127);
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
    this->p.neg  = (this->x > 127);

    this->pc++;
    return 2;
}

// CPY a
uint32_t OP_CC(NES* this) {
    OP_CPY(this, this->RAM[(VAL1 << 4) + VAL2]);

    this->pc += 3;
    return 4;
}

// CMP a
uint32_t OP_CD(NES* this) {
    OP_CMP(this, this->RAM[(VAL1 << 4) + VAL2]);

    this->pc += 3;
    return 4;
}

// DEC a
uint32_t OP_CE(NES* this) {
    OP_DEC(this, &this->RAM[(VAL1 << 4) + VAL2]);

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
    OP_CMP(this, index_ay(this, (VAL1 << 4) + VAL2));

    this->pc += 3;
    CHECK_PAGE(4);
}

// CMP a,x
uint32_t OP_DD(NES* this) {
    OP_CMP(this, index_ax(this, (VAL1 << 4) + VAL2));

    this->pc += 3;
    CHECK_PAGE(4);
}

// DEC a,x
uint32_t OP_DE(NES* this) {
    OP_DEC(this, &this->RAM[(VAL1 << 4) + VAL2 + this->x]);

    this->pc += 3;
    return 7;
}

// CPX #
uint32_t OP_E0(NES* this) {
    OP_CPX(this, VAL1);

    this->pc += 2;
    return 2;
}

// CPX d
uint32_t OP_E4(NES* this) {
    OP_CPX(this, this->RAM[VAL1]);

    this->pc += 2;
    return 3;
}

// CPX a
uint32_t OP_EC(NES* this) {
    OP_CPX(this, this->RAM[(VAL1 << 4) + VAL2]);

    this->pc += 3;
    return 4;
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