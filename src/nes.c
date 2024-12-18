#include "nes.h"

uint8_t* prgROM = NULL;
uint8_t* chrROM = NULL;
uint8_t* prgRAM = NULL;
uint8_t* prgNVRAM = NULL;
uint8_t* chrRAM = NULL;
uint8_t* chrNVRAM = NULL;
char* fileName;

int8_t special_plp;

const opcodeFunc opTable[] = {
    OP_00, OP_01, OP_STP, OP_03, OP_04, OP_05, OP_06, OP_07, OP_08, OP_09, OP_0A, OP_0B, OP_0C, OP_0D, OP_0E, OP_0F,
    OP_10, OP_11, OP_STP, OP_13, OP_14, OP_15, OP_16, OP_17, OP_18, OP_19, OP_1A, OP_1B, OP_1C, OP_1D, OP_1E, OP_1F,
    OP_20, OP_21, OP_STP, OP_23, OP_24, OP_25, OP_26, OP_27, OP_28, OP_29, OP_2A, OP_2B, OP_2C, OP_2D, OP_2E, OP_2F,
    OP_30, OP_31, OP_STP, OP_33, OP_34, OP_35, OP_36, OP_37, OP_38, OP_39, OP_3A, OP_3B, OP_3C, OP_3D, OP_3E, OP_3F,
    OP_40, OP_41, OP_STP, OP_43, OP_44, OP_45, OP_46, OP_47, OP_48, OP_49, OP_4A, OP_4B, OP_4C, OP_4D, OP_4E, OP_4F,
    OP_50, OP_51, OP_STP, OP_53, OP_54, OP_55, OP_56, OP_57, OP_58, OP_59, OP_5A, OP_5B, OP_5C, OP_5D, OP_5E, OP_5F,
    OP_60, OP_61, OP_STP, OP_63, OP_64, OP_65, OP_66, OP_67, OP_68, OP_69, OP_6A, OP_6B, OP_6C, OP_6D, OP_6E, OP_6F,
    OP_70, OP_71, OP_STP, OP_73, OP_74, OP_75, OP_76, OP_77, OP_78, OP_79, OP_7A, OP_7B, OP_7C, OP_7D, OP_7E, OP_7F,
    OP_80, OP_81, OP_82,  OP_83, OP_84, OP_85, OP_86, OP_87, OP_88, OP_89, OP_8A, OP_8B, OP_8C, OP_8D, OP_8E, OP_8F,
    OP_90, OP_91, OP_STP, OP_93, OP_94, OP_95, OP_96, OP_97, OP_98, OP_99, OP_9A, OP_9B, OP_9C, OP_9D, OP_9E, OP_9F,
    OP_A0, OP_A1, OP_A2,  OP_A3, OP_A4, OP_A5, OP_A6, OP_A7, OP_A8, OP_A9, OP_AA, OP_AB, OP_AC, OP_AD, OP_AE, OP_AF,
    OP_B0, OP_B1, OP_STP, OP_B3, OP_B4, OP_B5, OP_B6, OP_B7, OP_B8, OP_B9, OP_BA, OP_BB, OP_BC, OP_BD, OP_BE, OP_BF,
    OP_C0, OP_C1, OP_C2,  OP_C3, OP_C4, OP_C5, OP_C6, OP_C7, OP_C8, OP_C9, OP_CA, OP_CB, OP_CC, OP_CD, OP_CE, OP_CF,
    OP_D0, OP_D1, OP_STP, OP_D3, OP_D4, OP_D5, OP_D6, OP_D7, OP_D8, OP_D9, OP_DA, OP_DB, OP_DC, OP_DD, OP_DE, OP_DF,
    OP_E0, OP_E1, OP_E2,  OP_E3, OP_E4, OP_E5, OP_E6, OP_E7, OP_E8, OP_E9, OP_EA, OP_EB, OP_EC, OP_ED, OP_EE, OP_EF,
    OP_F0, OP_F1, OP_STP, OP_F3, OP_F4, OP_F5, OP_F6, OP_F7, OP_F8, OP_F9, OP_FA, OP_FB, OP_FC, OP_FD, OP_FE, OP_FF,
};

NES* newNES(void) {
    NES* nes = malloc(sizeof(NES));

    nes->sp = 0xFF;
    nes->pc = 0xFFFC;

    return nes;
}

void NESLoadHeader(NES* this, uint8_t header[]) {
    uint8_t flags = header[6];
    this->header.mirror_mode = (flags & 0x01) >> 0;
    this->header.has_battery = (flags & 0x02) >> 1;
    this->header.has_trainer = (flags & 0x04) >> 2;
    this->header.nametable   = (flags & 0x08) >> 3;
    this->header.mapper_lo   = (flags & 0xF0) >> 4;

    flags = header[7];
    this->header.console_type = (flags & 0x03);
    this->header.nes2         = (flags & 0x0C) >> 2;
    this->header.mapper_hi    = (flags & 0xF0) >> 4;

    if (this->header.nes2 != 2) {
        flags = header[8];
        this->header.prg_ram = flags;

        flags = header[9];
        this->header.tv_system = flags & 0x1;

        flags = header[10];
        this->header.tv_system_2  = (flags & 0x3);
        this->header.is_prg_ram   = (flags & 0x4) >> 2;
        this->header.bus_conflict = (flags & 0x8) >> 3;
    } else {
        flags = header[8];
        this->header.mapper_hiest = (flags & 0x0F);
        this->header.submapper    = (flags & 0xF0) >> 4;

        flags = header[9];
        this->header.prg_rom_hi   = (flags & 0x0F);
        this->header.chr_rom_hi   = (flags & 0xF0) >> 4;

        flags = header[10];
        this->header.prgram_size   = (flags & 0x0F);
        this->header.prgnvram_size = (flags & 0xF0) >> 4;

        flags = header[11];
        this->header.chrram_size   = (flags & 0x0F);
        this->header.chrnvram_size = (flags & 0xF0) >> 4;

        flags = header[12];
        this->header.proc_timing   = flags & 0x2;

        flags = header[13];
        if (this->header.console_type == 1) {
            this->header.vs_ppu_type = (flags & 0x0F);
            this->header.vs_hw_type  = (flags & 0xF0) >> 4;
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
    
    if (prgROM == NULL) {
        printf("ERROR: prgROM could not be allocated.");
        exit(EXIT_FAILURE);
    }
    if (chrROM == NULL) {
        printf("ERROR: chrROM could not be allocated.");
        exit(EXIT_FAILURE);
    }
    if (prgRAM == NULL) {
        printf("ERROR: prgRAM could not be allocated.");
        exit(EXIT_FAILURE);
    }
}

void NESLoadMem2(NES* this) {
    uint32_t ROMSize;

    if (this->header.prg_rom_hi == 0xF) {
        uint32_t mult = (this->header.prgrom_size & 0b00000011);
        uint32_t expn = (this->header.prgrom_size & 0b11111100) >> 2;

        if (expn > 26) {
            printf("ERROR: Could not read '%s', PRG-ROM size too high!");
            exit(EXIT_FAILURE);
        }

        ROMSize = (2 << expn) * (mult * 2 + 1);
    } else {
        ROMSize = this->header.prgrom_size + (this->header.prg_rom_hi << 0x8);
    }
    prgROM = malloc(ROMSize * sizeof(uint8_t) * 0x4000);

    if (this->header.chr_rom_hi == 0xF) {
        uint32_t mult = (this->header.chrrom_size & 0b00000011);
        uint32_t expn = (this->header.chrrom_size & 0b11111100) >> 2;

        if (expn > 26) {
            printf("ERROR: Could not read '%s', CHR-ROM size too high!");
            exit(EXIT_FAILURE);
        }
        
        ROMSize = (2 << expn) * (mult * 2 + 1);
    } else {
        ROMSize = this->header.chrrom_size + (this->header.chr_rom_hi << 0x8);
    }
    chrROM = malloc(ROMSize * sizeof(uint8_t) * 0x2000);

    prgRAM = malloc(64 << this->header.prgram_size);
    prgNVRAM = malloc(64 << this->header.prgnvram_size);
    chrRAM = malloc(64 << this->header.chrram_size);
    chrNVRAM = malloc(64 << this->header.chrnvram_size);
}

void NESLoadRAM(NES* this, uint8_t* bytes) {
    int rAddr = 0;
    int fAddr = 0;
    int i;

    // if (this->header.has_battery) {
    //     rAddr = 0x6000;
    // }

    if (this->header.has_trainer) {
        for (i = 0x7000; i < 0x7200; i++, fAddr++) {
            this->SRAM[i + 0x7000] = bytes[fAddr];
        }
    }

    if (this->header.prgrom_size == 1) {
        for (i = 0; i < 0x4000; i++, fAddr++) {
            prgROM[i] = this->PRGROM[i] = bytes[fAddr];
        }
    } else {
        for (i = 0; i < 0x8000; i++, fAddr++) {
            prgROM[i] = this->PRGROM[i] = bytes[fAddr];
        }
        for (; i < this->header.prgrom_size * 0x4000; i++, fAddr++) {
            prgROM[i] = bytes[fAddr];
        }
    }

    for (i = 0; i < this->header.chrrom_size * 0x2000; i++, fAddr++) {
        chrROM[i] = bytes[fAddr];
    }

    // temporary
    this->PPU.PPUSTATUS = 0xFF;
}

void NESLoadROM(NES* this, FILE* file, char const* filename, size_t filesize) {
    uint8_t header[0x10];
    uint8_t* fullFile;

    this->a = 0;
    this->x = 0;
    this->y = 0;

    this->pc = 0xFFFC;
    this->sp = 0xFF;
    this->p.flags = 0;

    this->cycleFunc = Cycle;

    fread(header, sizeof(uint8_t), 0x10, file);

    if (!(header[0] == 'N' && header[1] == 'E' && header[2] == 'S' && header[3] == '\x1A')) {
        printf("ERROR: Could not read '%s', incorrect format.", filename);
        exit(EXIT_FAILURE);
    }

    if (header[4] == 0) {
        printf("ERROR: Could not read '%s', invalid PRG ROM.", filename);
        exit(EXIT_FAILURE);
    }
    this->header.prgrom_size = header[4];

    if (header[5] == 0) {
        printf("ERROR: Could not read '%s', invalid CHR ROM.", filename);
        exit(EXIT_FAILURE);
    }
    this->header.chrrom_size = header[5];

    NESLoadHeader(this, header);

    if (this->header.nes2 != 2) {
        NESLoadMem(this);
    } else {
        NESLoadMem2(this);
    }

    fullFile = malloc((filesize - 0x10) * sizeof(uint8_t));
    if (fullFile == NULL) {
        printf("ERROR: file size could not be allocated.");
        exit(EXIT_FAILURE);
    }
    fread(fullFile, sizeof(uint8_t), filesize - 0x10, file);

    NESLoadRAM(this, fullFile);

    // mapper & ppu stuff
}

uint16_t lastPC;
void debugPrint(NES* this) {
    if (this->pc - lastPC > 1) {
        printf("%02X ", *read(this, lastPC + 1));
        if (this->pc - lastPC > 2) {
            printf("%02X", *read(this, lastPC + 2));
        }
    } else if ((int32_t)lastPC - this->pc > 0) {
        printf("#loop#");
    }
    printf("\n%X: %02X ", this->pc, *read(this, this->pc));
    lastPC = this->pc;
}

uint32_t Cycle(NES* this) {
    debugPrint(this);
    opTable[*read(this, this->pc)](this);
    // ppu cycle 3 times
}

uint32_t CyclePLP(NES* this) {
    debugPrint(this);
    uint32_t val = opTable[*read(this, this->pc)](this);

    this->p.intd = special_plp;
    this->cycleFunc = Cycle;

    return val;
}