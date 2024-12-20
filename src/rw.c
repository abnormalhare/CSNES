#include "rw.h"

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
        byte = this->RAM[pos];
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
    uint8_t val = read(this, this->pc++);
    printf("%02X ", val);
    return val;
}

void readVal(NES* this) {
    read(this, this->pc);
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
    if (addr != NULL) *addr = this->sp;
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
    uint8_t cpuAddr;

    if (addr != NULL) {
        read(this, (uint8_t)(byte + this->x) + (byte & 0xFF00));
        *addr = byte + this->x;
        val = read(this, byte + this->x);
    } else {
        cpuAddr = (uint8_t)(byte + this->x) + (byte & 0xFF00);
        val = read(this, cpuAddr);
        if (cpuAddr != byte + this->x) {
            cpuAddr = byte + this->x;
            val = read(this, cpuAddr);
        }
    }

    return val;
}

// 1 cycle, 2 if page crossed
uint8_t index_ay(NES* this, uint16_t byte, uint16_t* addr) {
    uint8_t val;
    uint8_t cpuAddr;

    if (addr != NULL) {
        read(this, (uint8_t)(byte + this->y) + (byte & 0xFF00));
        *addr = byte + this->y;
        val = read(this, byte + this->y);
    } else {
        cpuAddr = (uint8_t)(byte + this->y) + (byte & 0xFF00);
        val = read(this, cpuAddr);
        if (cpuAddr != byte + this->y) {
            cpuAddr = byte + this->y;
            val = read(this, cpuAddr);
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
        *addr = al + (ah << 8);
        val = read(this, *addr);
    } else {
        uint16_t falAddr = (uint8_t)(al + this->y) + (ah << 8);
        uint16_t truAddr = (al + this->y) + (ah << 8);

        val = read(this, falAddr);
        if (falAddr != truAddr) {
            val = read(this, truAddr);
        }
    }

    return val;
}

// 1 cycle
uint16_t index_axw(NES* this, uint16_t byte) {
    read(this, (uint8_t)(byte + this->x) + (byte & 0xFF00));
    return byte + this->x;
}

// 1 cycle
uint16_t index_ayw(NES* this, uint16_t byte) {
    read(this, (uint8_t)(byte + this->y) + (byte & 0xFF00));
    return byte + this->y;
}

// 3 cycles
uint16_t index_dxw(NES* this, uint8_t byte) {
    read(this, byte);
    uint8_t val = byte + this->x;
    uint8_t al = read(this, val);
    uint8_t ah = read(this, (uint8_t)(val + 1));

    return al + (ah << 8);
}

// 3 cycles
uint16_t index_dyw(NES* this, uint8_t byte) {
    uint8_t al = read(this, byte);
    uint8_t ah = read(this, (uint8_t)(byte + 1));
    read(this, (uint8_t)(al + this->y) + (ah << 8));

    return al + (ah << 8) + this->y;
}

// 1 cycle
uint8_t index_zxw(NES* this, uint8_t byte) {
    read(this, byte);
    return byte + this->x;
}