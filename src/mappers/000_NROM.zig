const std = @import("std");

const NES = @import("../nes.zig").NES;

const header = @import("../mapper.zig").header;
const prgROM = @import("../mapper.zig").prgROM;

pub fn init_000(nes: *NES) error{badPRGROM}!void {
    _ = nes;
}

pub fn read_000(index: u16) void {
    if (header.prgrom_size == 1) {
        return switch (index) {
            else => {},
            0x8000...0xBFFF => prgROM[index - 0x8000],
            0xC000...0xFFFF => prgROM[index - 0xC000],
        };
    } else if (header.prgrom_size == 2) {
        return switch (index) {
            else => {},
            0x8000...0xFFFF => prgROM[index - 0x8000],
        };
    } else {
        std.debug.print("ERROR: invalid prgm ROM size: {d} ({d})\n", .{@as(usize, header.prgrom_size) * 0x4000, header.prgrom_size});
        return error.badPRGROM;
    }
}

pub fn write_000(index: u16, data: u8) void {
    _ = index; _ = data;
}