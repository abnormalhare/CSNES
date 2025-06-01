const def = @import("defines.zig");
const NES = @import("nes.zig").NES;

pub const PPU = struct {
    nes: *NES,

    patternTbls: [0x2000]u8,

    nameTbl: [0x1000]u8,
    pallete: [32]def.Color,

    OAM: [4][0x40]u8,

    screen: [def.SCREEN_WIDTH * def.SCREEN_HEIGHT]def.Dot,
    scanline: i32,
    dot: i32,

    // registers
    v: u15,
    t: u15,
    x: u3,
    w: u1,

    // pins
    rw: u1,
    data: u8,
    addr: u3,

    // emu info
    wasRW: bool,
    cycleCount: u16,

    pub fn init(this: *PPU, nes: *NES) !void {
        this.nes = nes;
        this.v = 0;
        this.t = 0;
        this.x = 0;
        this.w = 0;
        this.cycleCount = 0;
        this.scanline = 0;
        this.dot = 0;
    }

    pub fn run(this: *PPU) void {
        if (!this.wasRW) return;

        this.wasRW = false;

        switch (this.addr) {
            0 => {},
            1 => {},
            2 => {},
            3 => {},
            4 => {},
            5 => {},
            6 => {},
            7 => {},
        }

        this.nes.data = this.data;
    }

    pub fn read(this: *PPU, index: u3) u8 {
        this.rw = 1;
        this.addr = index;

        this.wasRW = true;

        return switch (index) {
            else => this.data,
            // 2 => {},
        };
    }

    pub fn write(this: *PPU, index: u3, data: u8) void {
        this.rw = 0;
        this.addr = index;
        this.data = data;
        
        this.wasRW = true;
    }
};
