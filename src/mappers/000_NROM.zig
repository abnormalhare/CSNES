const std = @import("std");

const NES = @import("../nes.zig").NES;
const def = @import("../defines.zig");

pub fn read_000(index: u16) u8 {
    var ret: u8 = 0;
    if (def.header.prgrom_size == 1) {
        ret = switch (index) {
            else => 0,
            0x8000...0xBFFF => def.prgROM[index - 0x8000],
            0xC000...0xFFFF => def.prgROM[index - 0xC000],
        };
    } else if (def.header.prgrom_size == 2) {
        ret = switch (index) {
            else => 0,
            0x8000...0xFFFF => def.prgROM[index - 0x8000],
        };
    }

    if ((def.header.is_nes2 != 2 and def.header.byte8.prgram_size == 1) or (def.header.is_nes2 == 2 and def.header.byteA.two.prgram_size == 7)) {
        ret = switch (index) {
            else => ret,
            0x6000...0x7FFF => def.prgRAM[index - 0x6000],
        };
    }

    return ret;
}

pub fn write_000(index: u16, data: u8) void {
    if ((def.header.is_nes2 != 2 and def.header.byte8.prgram_size == 1) or (def.header.is_nes2 == 2 and def.header.byteA.two.prgram_size == 7)) {
        switch (index) {
            else => {},
            0x6000...0x7FFF => def.prgRAM[index - 0x6000] = data,
        }
    }
}
