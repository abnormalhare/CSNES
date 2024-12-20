#include "opfuncs.h"

uint32_t page_crossed = 0;

void write(NES* this, uint16_t addr, uint8_t byte) {
    uint16_t pos;
    if (addr < 0x2000) {
        pos = addr % 0x800;
        this->RAM[pos] = byte;
    } else if (addr < 0x4000) {
        pos = (addr - 0x2000) % 8;
        switch (pos) {
            case 0: this->PPURegs.PPUCTRL   = byte; break;
            case 1: this->PPURegs.PPUMASK   = byte; break;
            case 3: this->PPURegs.OAMADDR   = byte; break;
            case 4: this->PPURegs.OAMDATA   = byte; break;
            case 5: this->PPURegs.PPUSCROLL = byte; break;
            case 6: this->PPURegs.PPUADDR   = byte; break;
            case 7: this->PPURegs.PPUDATA   = byte; break;
        }
    } else if (addr < 0x8000) {

    } else {
        pos = (addr - 0x8000);
        if (this->header.prgrom_size == 1) { // temporary NROM case
            pos %= 0x4000;
        }

        this->PRGROM[pos] = byte;
    }

    this->cycleCount++;
}

uint8_t read(NES* this, uint16_t addr) {
    uint16_t pos;
    uint8_t byte;

    if (addr < 0x2000) {
        pos = addr % 0x800;
        byte = &this->RAM[pos];
    } else if (addr < 0x4000) {
        pos = (addr - 0x2000) % 8;
        switch (pos) {
            case 2: byte = this->PPURegs.PPUSTATUS; break;
            case 4: byte = this->PPURegs.OAMDATA;   break;
            case 7: byte = this->PPURegs.PPUDATA;   break;
            default:
                printf("ERROR: read on non-readable PPU instruction (temporary)");
                exit(EXIT_FAILURE);
        }
    } else if (addr < 0x8000) {

    } else {
        pos = (addr - 0x8000);
        if (this->header.prgrom_size == 1) { // temporary NROM case
            pos %= 0x4000;
        }

        byte = this->PRGROM[pos];
    }

    this->cycleCount++;
    return byte;
}

// 1 cycle
uint8_t getVal(NES* this) {
    return read(this, this->pc++);
}

uint8_t readVal(NES* this) {
    return read(this, this->pc);
}

void ntrnlop(NES* this) {
    this->cycleCount++;
}

void setupPC(NES* this) {
    this->pc = read(this, this->pc) + (read(this, this->pc + 1) << 8);
}

void push(NES* this, uint8_t byte) {
    write(this, this->sp--, byte);
}

// 1 cycle
uint8_t pop(NES* this, uint16_t* addr) {
    *addr = this->sp;
    return read(this, this->sp++);
}

uint8_t index_z(NES* this, uint8_t byte, uint16_t* addr) {
    *addr = byte;
    return read(this, *addr);
}

// 1 cycle
uint8_t index_zx(NES* this, uint8_t byte, uint16_t* addr) {
    *addr = (uint8_t)(this->x + byte);
    return read(this, *addr);
}

// 1 cycle
uint8_t index_zy(NES* this, uint8_t byte, uint16_t* addr) {
    *addr = (uint8_t)(this->y + byte);
    return read(this, *addr);
}

// 1 cycle
uint8_t index_a(NES* this, uint16_t byte, uint16_t* addr) {
    *addr = byte;
    return read(this, *addr);
}

// 1 cycle, 2 if page crossed
uint8_t index_ax(NES* this, uint16_t byte, uint16_t* addr) {
    uint8_t val;

    if (addr != NULL) {
        read(this, (uint8_t)(byte + this->x) + (byte & 0xF0));
        val = read(this, byte + this->x);
    } else {
        *addr = (uint8_t)(byte + this->x) + (byte & 0xF0);
        val = read(this, *addr);
        if (*addr != byte + this->x) {
            *addr = byte + this->x;
            val = read(this, *addr);
        }
    }

    return val;
}

// 1 cycle, 2 if page crossed
uint8_t index_ay(NES* this, uint16_t byte, uint16_t* addr) {
    uint8_t val;

    if (addr != NULL) {
        read(this, (uint8_t)(byte + this->y) + (byte & 0xF0));
        val = read(this, byte + this->y);
    } else {
        *addr = (uint8_t)(byte + this->y) + (byte & 0xF0);
        val = read(this, *addr);
        if (*addr != byte + this->y) {
            *addr = byte + this->y;
            val = read(this, *addr);
        }
    }

    return val;
}

// 4 cycles
uint8_t index_dx(NES* this, uint8_t byte, uint16_t* addr) {
    read(this, byte);
    uint8_t val = byte + this->x;
    uint8_t al = read(this, val);
    uint8_t ah = read(this, (uint8_t)(val + 1));

    *addr = al + (ah << 8);
    return read(this, *addr);
}

// 4 cycles, 5 if page crossed
uint8_t index_dy(NES* this, uint8_t byte, uint16_t* addr) {
    uint8_t al = read(this, byte);
    uint8_t ah = read(this, (uint8_t)(byte + 1));
    uint8_t val;

    if (addr != NULL) {
        read(this, (uint8_t)(al + this->y) + (ah << 8));
        val = read(this, al + (ah << 8));
    } else {
        *addr = (uint8_t)(al + this->y) + (ah << 8);
        uint16_t truAddr = (al + this->y) + (ah << 8);

        val = read(this, *addr);
        if (*addr != truAddr) {
            *addr = truAddr;
            val = read(this, *addr);
        }
    }

    return val;
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

void OP_DCP(NES* this, uint8_t* mem) {
    OP_DEC(this, mem);
    OP_CMP(this, *mem);
}

void OP_DEC(NES* this, uint8_t* mem) {
    *mem--;

    this->p.zero = (*mem == 0);
    this->p.neg  = (*mem > 0x7F);
}

void OP_EOR(NES* this, uint8_t mem) {
    this->a ^= mem;

    this->p.zero = (this->a == 0);
    this->p.neg  = (this->a > 0x7F);
}

void OP_INC(NES* this, uint8_t* mem) {
    *mem++;

    this->p.zero = (*mem == 0);
    this->p.neg  = (*mem > 0x7F);
}

void OP_ISC(NES* this, uint8_t* mem) {
    OP_INC(this, mem);
    OP_SBC(this, *mem);
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

void OP_RLA(NES* this, uint8_t* mem) {
    OP_ROL(this, mem);
    OP_AND(this, *mem);
}

void OP_RRA(NES* this, uint8_t* mem) {
    OP_ROR(this, mem);
    OP_ADC(this, *mem);
}

void OP_SBC(NES* this, uint8_t mem) {
    uint8_t val = this->a;
    this->a -= mem + ~this->p.carry;

    this->p.carry = (this->a >= val && mem != 0); // check for underflow. it could underflow to the same value, so check if that isnt possible
    this->p.zero  = (this->a == 0);
    this->p.over  = ((this->a ^ val) & (this->a ^ (~mem)) & 0x80) == 0x80;
    this->p.neg   = (this->a > 0x7F);
}

void OP_SLO(NES* this, uint8_t* mem) {
    OP_ASL(this, mem);
    OP_ORA(this, *mem);
}

void OP_SRE(NES* this, uint8_t* mem) {
    OP_LSR(this, mem);
    OP_EOR(this, *mem);
}