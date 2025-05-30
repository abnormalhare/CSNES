const std = @import("std");

pub const Status = extern union {
    flags: packed struct { carry: u1, zero: u1, intd: u1, dec: u1, brk: u1, unused: u1, over: u1, neg: u1 },
    all: u8,
};

pub const Header = struct {
    prgrom_size: u8,
    chrrom_size: u8,

    // 0x6
    mirror: u1,
    battery: u1,
    trainer: u1,
    nametable: u1,
    mapper_lo: u4,

    // 0x7
    console_type: u2,
    is_nes2: u2,
    mapper_hi: u4,

    byte8: union {
        prgram_size: u8,
        two: struct {
            submapper: u4,
            mapper_hiest: u4,
        },
    },

    byte9: union {
        frequency: u1,
        two: struct {
            prgrom_hi: u4,
            chrrom_hi: u4,
        },
    },

    byteA: union { one: struct {
        frequency_2: u2,
        __reserved: u2,
        hasnt_prgram: u1,
        bus_conflict: u1,
    }, two: struct {
        prgram_size: u4,
        prgnvram_size: u4,
    } },
    // ONLY NES2.0
    // 0xB
    chrram_size: u4,
    chrnvram_size: u4,

    // future support
};

const Color = struct { v: u6 };

pub const Dot = struct { pallete: u6, emphesis: u3 };

pub const SCREEN_WIDTH: i32 = 341;
pub const SCREEN_HEIGHT: i32 = 262;

pub const PPU = struct {
    patternTbls: []u8,

    nameTbl: [0x1000]u8,
    pallete: [32]Color,

    OAM: [4][0x40]u8,

    screen: [SCREEN_WIDTH * SCREEN_HEIGHT]Dot,
    scanline: i32,
    dot: i32,

    PPUCTRL: u8,
    PPUMASK: u8,
    PPUSTATUS: u8,
    OAMADDR: u8,
    OAMDATA: u8,
    PPUSCROLL: u8,
    PPUDATA: u8,

    isEvenFrame: bool
};

pub const addr_bus = extern union {
    full: u16,
    half: extern struct {
        high: u8,
        low: u8,
    },
};

pub var mapper: u8 = undefined;
pub var header: Header = undefined;
pub var trainer: []u8 = undefined;
pub var prgROM: []u8 = undefined;
pub var chrROM: []u8 = undefined;
pub var prgRAM: []u8 = undefined;
pub var chrRAM: []u8 = undefined;
pub var prgNVRAM: []u8 = undefined;
pub var chrNVRAM: []u8 = undefined;