const NES = @import("nes.zig").NES;
const mapper = @import("mapper.zig");

pub fn read(nes: *NES, index: u16) u8 {
    var ret: u16 = 0;
    ret = switch (index) {
        0x0000...0x1FFF => nes.RAM[index % 0x0800],
        0x2000...0x3FFF => (index % 8) + 0x2000,
        0x4000...0x401F => nes.data,
        0x4020...0xFFFF => mapper.read(index),
    };
    ret = switch (ret) {
        else => ret,
        0x2000 => nes.data,
        0x2001 => nes.data,
        0x2002 => nes.ppu.PPUSTATUS,
        0x2003 => nes.data,
        0x2004 => nes.ppu.OAMDATA,
        0x2005 => nes.data,
        0x2006 => nes.data,
        0x2007 => nes.ppu.PPUDATA,
    };

    return @truncate(ret);
}

pub fn write(nes: *NES, index: u16, data: u8) void {
    switch (index) {
        0x0000...0x1FFF => {
            nes.RAM[index % 0x0800] = data;
        },
        0x2000 => nes.ppu.PPUCTRL = data,
        0x2001 => nes.ppu.PPUMASK = data,
        0x2003 => nes.ppu.OAMADDR = data,
        // 0x2004 => nes.ppu.wScroll(),
        // 0x2005 => nes.ppu.wAddr(),
        0x2006 => nes.ppu.PPUDATA = data,
        0x4020...0xFFFF => mapper.write(index, data),
        else => {}

    }
    nes.data = data;
}