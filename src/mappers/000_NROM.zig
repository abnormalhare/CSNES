const std = @import("std");

const NES = @import("../nes.zig").NES;
const def = @import("../defines.zig");

pub fn read_000(index: u16) u8 {
    if (def.header.prgrom_size == 1) {
        return switch (index) {
            else => 0,
            0x8000...0xBFFF => def.prgROM[index - 0x8000],
            0xC000...0xFFFF => def.prgROM[index - 0xC000],
        };
    } else if (def.header.prgrom_size == 2) {
        return switch (index) {
            else => 0,
            0x8000...0xFFFF => def.prgROM[index - 0x8000],
        };
    }

    return 0;
}

pub fn write_000(index: u16, data: u8) void {
    _ = index; _ = data;
}