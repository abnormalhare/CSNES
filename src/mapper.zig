const std = @import("std");
const alloc = @import("global").alloc;

const def = @import("defines.zig");

fn headerChecksum(hdr: *[4]u8) bool {
    return  hdr[0] == 'N'
        and hdr[1] == 'E'
        and hdr[2] == 'S'
        and hdr[3] == '\x1A';
}

pub fn initMapper(file: std.fs.File) !bool {
    var fileHeader: [0x10]u8 = [_]u8{0} ** 0x10;
    
    const len: usize = try file.read(&fileHeader);
    if (len != fileHeader.len) {
        std.debug.print("ERROR: File is not an NES file.\n", .{});
        return false;
    }
    if (!headerChecksum(fileHeader[0..4])) {
        std.debug.print("ERROR: File does not contain a valid header.\n", .{});
        return false;
    }
    if (fileHeader[4] == 0) {
        std.debug.print("ERROR: Invalid PRG ROM size {d}.\n", .{fileHeader[4]});
        return false;
    }
    if (fileHeader[5] == 0) {
        std.debug.print("ERROR: Invalid CHR ROM size {d}.\n", .{fileHeader[5]});
        return false;
    }

    loadHeader(&fileHeader);
    if (def.header.is_nes2 != 2) {
        loadMem() catch return false;
    } else {
        loadMem2() catch return false;
    }

    if (def.header.trainer == 1) {
        def.trainer = try alloc.alloc(u8, 0x200);
        _ = try file.read(def.trainer);
    }

    _ = try file.read(def.prgROM);
    _ = try file.read(def.chrROM);

    if ((def.header.console_type & 0x2) == 2) {
        std.debug.print("UNSUPPORTED: PlayChoice INST-ROM. Sorry /shrug\n", .{});
    }

    return true;
}

pub fn loadHeader(flags: []u8) void {
    def.header.prgrom_size = flags[4];
    def.header.chrrom_size = flags[5];

    def.header.mirror =    @intCast((flags[6] & 0x01) >> 0);
    def.header.battery =   @intCast((flags[6] & 0x02) >> 1);
    def.header.trainer =   @intCast((flags[6] & 0x04) >> 2);
    def.header.nametable = @intCast((flags[6] & 0x08) >> 3);
    def.header.mapper_lo = @intCast((flags[6] & 0xF0) >> 4);

    def.header.console_type = @intCast((flags[7] & 0x03) >> 0);
    def.header.is_nes2      = @intCast((flags[7] & 0x0C) >> 2);
    def.header.mapper_hi    = @intCast((flags[7] & 0xF0) >> 4);

    if (def.header.is_nes2 != 2) {
        def.header.byte8.prgram_size = flags[8];

        def.header.byte9.frequency = @intCast(flags[9] & 1);

        def.header.byteA.one.frequency_2  = @intCast((flags[10] & 0x03) >> 0);
        def.header.byteA.one.hasnt_prgram = @intCast((flags[10] & 0x04) >> 2);
        def.header.byteA.one.bus_conflict = @intCast((flags[10] & 0x08) >> 3);
        return;
    }

    // is nes2.0
    def.header.byte8.two.mapper_hiest = @intCast((flags[8] & 0x0F) >> 0);
    def.header.byte8.two.submapper    = @intCast((flags[8] & 0xF0) >> 4);

    def.header.byte9.two.prgrom_hi = @intCast((flags[9] & 0x0F) >> 0);
    def.header.byte9.two.chrrom_hi = @intCast((flags[9] & 0xF0) >> 4);

    def.header.byteA.two.prgram_size =   @intCast((flags[10] & 0x0F) >> 0);
    def.header.byteA.two.prgnvram_size = @intCast((flags[10] & 0xF0) >> 4);
    
    def.header.chrram_size =   @intCast((flags[11] & 0x0F) >> 0);
    def.header.chrnvram_size = @intCast((flags[11] & 0xF0) >> 4);

    // to be later implemented

    def.mapper = @intCast(def.header.mapper_lo + (@as(u8, def.header.mapper_hi) << 4));
}

pub fn loadMem() !void {
    def.prgROM = try alloc.alloc(u8, @as(usize, def.header.prgrom_size) * 0x4000);
    def.chrROM = try alloc.alloc(u8, @as(usize, def.header.chrrom_size) * 0x2000);
    def.prgRAM = try alloc.alloc(u8, @as(usize, def.header.byte8.prgram_size) * 0x2000);
}

pub fn calcSize(size: u8, hi_size: u4) error{SizeTooBig}!u32 {
    var retSize: u32 = 0;

    if (hi_size == 0xF) {
        const mult: u32 = (size & 0x03);
        const expn: u5 = @intCast((size & 0xFC) >> 2);
        if (expn > 26) {
            std.debug.print("ERROR: PRG ROM size is too high.", .{});
            return error.SizeTooBig;
        }

        const shift: u32 = 2;
        retSize = (shift << expn) * (mult * 2 + 1);
    } else {
        retSize = size + (@as(u32, hi_size) << 0x8);
    }
    return retSize;
}

fn loadMem2() !void {
    var size: u32 = try calcSize(def.header.prgrom_size, def.header.byte9.two.prgrom_hi);
    def.prgROM = try alloc.alloc(u8, size * 0x4000);

    size = try calcSize(def.header.chrrom_size, def.header.byte9.two.chrrom_hi);
    def.chrROM = try alloc.alloc(u8, size * 0x2000);

    const shift: usize = 64;
    def.prgRAM = try alloc.alloc(u8, shift << def.header.byteA.two.prgram_size);
    def.chrRAM = try alloc.alloc(u8, shift << def.header.chrram_size);
    def.prgNVRAM = try alloc.alloc(u8, shift << def.header.byteA.two.prgnvram_size);
    def.chrNVRAM = try alloc.alloc(u8, shift << def.header.chrnvram_size);
}

pub fn read(index: u16) u8 {
    const mapper_num: u8 = @as(u8, def.header.mapper_lo + (@as(u8, def.header.mapper_hi) << 4));
    return mapperReadTable[mapper_num](index);
}

pub fn write(index: u16, data: u8) void {
    const mapper_num: u8 = @as(u8, def.header.mapper_lo + (@as(u8, def.header.mapper_hi) << 4));
    mapperWriteTable[mapper_num](index, data);
}

const mapperReadTable: [1]*const fn (u16) u8 = .{
    @import("mappers/000_NROM.zig").read_000,
};

const mapperWriteTable: [1]*const fn (u16, u8) void = .{
    @import("mappers/000_NROM.zig").write_000,
};