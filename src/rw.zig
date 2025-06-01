const NES = @import("nes.zig").NES;
const mapper = @import("mapper.zig");

pub fn read(nes: *NES, index: u16) u8 {
    return switch (index) {
        0x0000...0x1FFF => nes.RAM[index % 0x0800],
        0x2000...0x3FFF => nes.ppu.read(@intCast(index % 8)),
        0x4000...0x401F => nes.data,
        0x4020...0xFFFF => mapper.read(index),
    };
}

pub fn write(nes: *NES, index: u16, data: u8) void {
    switch (index) {
        0x0000...0x1FFF => {
            nes.RAM[index % 0x0800] = data;
        },
        0x2000...0x3FFF => nes.ppu.write(@intCast(index % 8), data),
        0x4020...0xFFFF => mapper.write(index, data),
        else => {}

    }
    nes.data = data;
}