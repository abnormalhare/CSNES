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

void OP_AND(NES* this, uint16_t mem) {
    this->a &= mem;

    this->p.zero = (this->a == 0);
    this->p.neg  = (this->a > 0x7F);
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
    OP_AND(this, (VAL1 << 4) + VAL2);
    this->pc += 3;
    return 4;
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