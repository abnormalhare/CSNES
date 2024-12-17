#include "opfuncs.h"

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

void OP_ASL(NES* this, uint8_t* mem) {
    this->p.flags = (*mem > 0x7F);
    *mem <<= 1;

    this->p.zero  = (*mem == 0);
    this->p.neg   = (*mem > 0x7F);
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
    this->p.neg  = (*mem > 0x7F);
}

void OP_INC(NES* this, uint8_t* mem) {
    *mem++;

    this->p.zero = (*mem == 0);
    this->p.neg  = (*mem > 0x7F);
}

void OP_EOR(NES* this, uint8_t mem) {
    this->a ^= mem;

    this->p.zero = (this->a == 0);
    this->p.neg  = (this->a > 0x7F);
}

void OP_LAX(NES* this, uint8_t mem) {
    OP_LDA(this, mem);
    this->x = this->a;
}

void OP_LDA(NES* this, uint8_t mem) {
    this->a = mem;

    this->p.zero = (this->a == 0);
    this->p.neg  = (this->a > 0x7F);
}

void OP_LDX(NES* this, uint8_t mem) {
    this->x = mem;

    this->p.zero = (this->x == 0);
    this->p.neg  = (this->x > 0x7F);
}

void OP_LDY(NES* this, uint8_t mem) {
    this->y = mem;

    this->p.zero = (this->y == 0);
    this->p.neg  = (this->y > 0x7F);
}

void OP_LSR(NES* this, uint8_t* mem) {
    this->p.carry = *mem & 1;
    *mem >>= 1;
    
    this->p.zero  = (*mem == 0);
    this->p.neg   = (*mem > 0x7F);
}

void OP_ORA(NES* this, uint8_t mem) {
    this->a |= mem;

    this->p.zero = (mem == 0);
    this->p.neg  = (mem > 0x7F);
}

void OP_ROL(NES* this, uint8_t* mem) {
    uint8_t carry = (*mem > 0x7F);
    this->p.carry = carry;

    *mem <<= 1;
    *mem |= carry;

    this->p.zero  = (*mem == 0);
    this->p.neg   = (*mem > 0x7F);
}

void OP_ROR(NES* this, uint8_t* mem) {
    uint8_t carry = (*mem & 1);
    this->p.carry = carry;

    *mem >>= 1;
    *mem |= carry << 7;

    this->p.zero  = (*mem == 0);
    this->p.neg   = (*mem > 0x7F);
}

void OP_SBC(NES* this, uint8_t mem) {
    uint8_t val = this->a;
    this->a -= mem + ~this->p.carry;

    this->p.carry = (this->a >= val && mem != 0); // check for underflow. it could underflow to the same value, so check if that isnt possible
    this->p.zero  = (this->a == 0);
    this->p.over  = ((this->a ^ val) & (this->a ^ (~mem)) & 0x80) == 0x80;
    this->p.neg   = (this->a > 0x7F);
}