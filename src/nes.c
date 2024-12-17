#include "nes.h"

const opcodeFunc opTable[] = {
    OP_00, OP_01, OP_02, OP_STP, OP_04, OP_05, OP_06, OP_07, OP_08, OP_09, OP_0A, OP_0B, OP_0C, OP_0D, OP_0E, OP_0F,
    OP_10, OP_11, OP_12, OP_STP, OP_14, OP_15, OP_16, OP_17, OP_18, OP_19, OP_1A, OP_1B, OP_1C, OP_1D, OP_1E, OP_1F,
    OP_20, OP_21, OP_22, OP_STP, OP_24, OP_25, OP_26, OP_27, OP_28, OP_29, OP_2A, OP_2B, OP_2C, OP_2D, OP_2E, OP_2F,
    OP_30, OP_31, OP_32, OP_STP, OP_34, OP_35, OP_36, OP_37, OP_38, OP_39, OP_3A, OP_3B, OP_3C, OP_3D, OP_3E, OP_3F,
};

NES* newNES(void) {
    NES* nes = malloc(sizeof(NES));

    nes->sp = 0xFF;
    nes->pc = 0xFFFC;

    return nes;
}

void NESLoadHeader(NES* this, uint8_t* header[]) {
    uint8_t flags = header[6];
    this->header.mirror_mode = flags & 0x01 >> 0;
    this->header.has_battery = flags & 0x02 >> 1;
    this->header.has_trainer = flags & 0x04 >> 2;
    this->header.nametable   = flags & 0x08 >> 3;
    this->header.mapper_lo   = flags & 0xF0 >> 4;

    flags = header[7];
    this->header.console_type = flags & 0x03;
    this->header.nes2         = flags & 0x0C >> 2;
    this->header.mapper_hi    = flags & 0xF0 >> 4;

    if (this->header.nes2 != 2) {
        flags = header[8];
        this->header.prg_ram = flags;

        flags = header[9];
        this->header.tv_system = flags & 0x1;

        flags = header[10];
        this->header.tv_system_2  = flags & 0x3;
        this->header.is_prg_ram   = flags & 0x4 >> 2;
        this->header.bus_conflict = flags & 0x8 >> 3;
    } else {
        flags = header[8];
        this->header.mapper_hiest = flags & 0x0F;
        this->header.submapper    = flags & 0xF0;

        flags = header[9];
        this->header.prg_rom_hi   = flags & 0x0F;
        this->header.chr_rom_hi   = flags & 0xF0;

        flags = header[10];
        this->header.prgram_size   = flags & 0x0F;
        this->header.prgnvram_size = flags & 0xF0;

        flags = header[11];
        this->header.chrram_size   = flags & 0x0F;
        this->header.chrnvram_size = flags & 0xF0;

        flags = header[12];
        this->header.proc_timing   = flags & 0x2;

        flags = header[13];
        if (this->header.console_type == 1) {
            this->header.vs_ppu_type = flags & 0x0F;
            this->header.vs_hw_type  = flags & 0xF0;
        } else if (this->header.console_type == 3) {
            this->header.ext_cons_type = flags & 0x0F;
        }

        flags = header[14];
        this->header.num_roms = flags & 0x3;

        flags = header[15];
        this->header.def_exp_dev = flags & 0x3F;
    }
}

void NESLoadMem(NES* this) {
    prgROM = malloc(this->header.prgrom_size * 0x4000);
    chrROM = malloc(this->header.chrrom_size * 0x2000);
    prgRAM = malloc(this->header.prgram_size * 0x2000);
}

void NESLoadMem2(NES* this) {
    uint32_t ROMSize;

    if (this->header.prg_rom_hi == 0xF) {
        uint32_t mult = (this->header.prgrom_size & 0b00000011);
        uint32_t expn = (this->header.prgrom_size & 0b11111100);

        if (expn > 26) {
            printf("ERROR: Could not read '%s', PRG-ROM size too high!");
            return;
        }

        ROMSize = (2 << expn) * (mult * 2 + 1);
    } else {
        ROMSize = this->header.prgrom_size + (this->header.prg_rom_hi << 0x8);
    }
    prgROM = malloc(ROMSize * sizeof(uint8_t));

    if (this->header.chr_rom_hi == 0xF) {
        uint32_t mult = (this->header.chrrom_size & 0b00000011);
        uint32_t expn = (this->header.chrrom_size & 0b11111100);

        if (expn > 26) {
            printf("ERROR: Could not read '%s', CHR-ROM size too high!");
            return;
        }
        
        ROMSize = (2 << expn) * (mult * 2 + 1);
    } else {
        ROMSize = this->header.chrrom_size + (this->header.chr_rom_hi << 0x8);
    }
    chrROM = malloc(ROMSize * sizeof(uint8_t));

    prgRAM = malloc(64 << this->header.prgram_size);
    prgNVRAM = malloc(64 << this->header.prgnvram_size);
    chrRAM = malloc(64 << this->header.chrram_size);
    chrNVRAM = malloc(64 << this->header.chrnvram_size);
}

void NESLoadROM(NES* this, char const* filename) {
    FILE* file = fopen(filename, "rb");
    uint8_t header[0x10];

    fread(header, sizeof(uint8_t), 16, file);

    if (!(header[0] == 'N' && header[1] == 'E' && header[2] == 'S' && header[3] == '\x1A')) {
        printf("ERROR: Could not read '%s', incorrect format.", filename);
        return;
    }

    if (header[4] == 0) {
        printf("ERROR: Could not read '%s', invalid PRG ROM.", filename);
        return;
    }
    this->header.prgrom_size = header[4];

    if (header[5] == 0) {
        printf("ERROR: Could not read '%s', invalid CHR ROM.", filename);
    }
    this->header.chrrom_size = header[5];

    NESLoadHeader(this, header);

    if (this->header.nes2 != 2) {
        NESLoadMem(this);
    } else {
        NESLoadMem2(this);
    }
}

uint32_t Cycle(NES* this) {
    return opTable[this->RAM[this->pc]];
}