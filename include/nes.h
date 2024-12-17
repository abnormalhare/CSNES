#ifndef NES_H
#define NES_H

#include <stdint.h>
#include <malloc.h>
#include <stdio.h>
#include <math.h>

typedef struct _status {
    union {
        uint8_t carry : 1;
        uint8_t zero  : 1;
        uint8_t intd  : 1;
        uint8_t dec   : 1;
        uint8_t brk   : 1;
        uint8_t unused : 1;
        uint8_t over  : 1;
        uint8_t neg   : 1;
    };
    uint8_t flags;
} Status;

typedef struct _header {
    // header bytes //
    uint8_t prgrom_size;
    uint8_t chrrom_size;
    // 0x6
    uint8_t mirror_mode : 1; // 0: horizontal, 1: vertical
    uint8_t has_battery : 1; // 1: 0x6000 - 0x7FFF has persistant memory
    uint8_t has_trainer : 1; // 1: 0x7000 - 0x71FF has trainer
    uint8_t nametable   : 1; // 1: has alternate nametable layout (4-screen / 1-screen)
    uint8_t mapper_lo   : 4; // mapper lower nybble
    // 0x7
    uint8_t console_type: 2; // vs system, playchoice, etc.
    uint8_t nes2        : 2; // if == 2, uses NES2.0 format
    uint8_t mapper_hi   : 4; // mapper uppder nybble
    // 0x8
    union {
        uint8_t prg_ram;
        // NES2.0 //
        struct {
            uint8_t submapper    : 4;
            uint8_t mapper_hiest : 4;
        };
    };
    // 0x9
    union {
        struct {
            uint8_t tv_system : 1; // 0: NTSC, 1: PAL
            uint8_t reserved  : 7;
        };
        // NES2.0 //
        struct {
            uint8_t prg_rom_hi : 4;
            uint8_t chr_rom_hi : 4;
        };
    };
    // 0xA
    union {
        struct {
            uint8_t tv_system_2 : 2; // 0: NTSC, 2: PAL, else compat
            uint8_t reserved2   : 2;
            uint8_t is_prg_ram  : 1;
            uint8_t bus_conflict: 1;
        };
        // NES2.0 //
        struct {
            uint8_t prgram_size : 4; // prg_ram_size << prgram_size
            uint8_t prgnvram_size : 4;
        };
    };
    // NES2.0 //
    // 0xB
    uint8_t chrram_size   : 4;
    uint8_t chrnvram_size : 4;
    // 0xC
    uint8_t proc_timing   : 2; // 0: ntsc, 1: pal, 2: multi-region, 3: dendy
    uint8_t reserved3     : 6;
    // 0xD
    union {
        struct {
            uint8_t vs_ppu_type : 4;
            uint8_t vs_hw_type  : 4;
        };
        uint8_t ext_cons_type   : 4;
    };
    // 0xE
    uint8_t num_roms : 2;
    uint8_t reserved4 : 6;
    // 0xF
    uint8_t def_exp_dev : 6;
    uint8_t reserved5 : 2;
} Header;

typedef struct _NES {
    uint8_t a;
    uint8_t x;
    uint8_t y;

    uint16_t pc;
    uint8_t sp;
    Status p;

    uint8_t RAM[0x10000];
    uint8_t PPU[0x10000];

    Header header;
} NES;

typedef uint32_t (*opcodeFunc)(NES*);

#include "opcodes.h"

extern uint8_t* prgROM;
extern uint8_t* chrROM;
extern uint8_t* prgRAM;
extern uint8_t* prgNVRAM;
extern uint8_t* chrRAM;
extern uint8_t* chrNVRAM;
extern char* fileName;

NES* newNES(void);
void NESLoadROM(NES* this, char const* filename);
uint32_t Cycle(NES* this);

#endif