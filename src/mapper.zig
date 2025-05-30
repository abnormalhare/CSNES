const std = @import("std");
const alloc = @import("global").alloc;

const def = @import("defines.zig");

pub var mapper: u8 = undefined;
pub var header: def.Header = undefined;
pub var prgROM: []u8 = undefined;
pub var chrROM: []u8 = undefined;
pub var prgRAM: []u8 = undefined;
pub var chrRAM: []u8 = undefined;
pub var prgNVRAM: []u8 = undefined;
pub var chrNVRAM: []u8 = undefined;

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
    if (header.is_nes2 != 2) {
        loadMem() catch return false;
    } else {
        loadMem2() catch return false;
    }

    return true;
}

pub fn loadHeader(flags: []u8) void {
    header.prgrom_size = flags[4];
    header.chrrom_size = flags[5];

    header.mirror =    @intCast((flags[6] & 0x01) >> 0);
    header.battery =   @intCast((flags[6] & 0x02) >> 1);
    header.trainer =   @intCast((flags[6] & 0x04) >> 2);
    header.nametable = @intCast((flags[6] & 0x08) >> 3);
    header.mapper_lo = @intCast((flags[6] & 0xF0) >> 4);

    header.console_type = @intCast((flags[7] & 0x03) >> 0);
    header.is_nes2      = @intCast((flags[7] & 0x0C) >> 2);
    header.mapper_hi    = @intCast((flags[7] & 0xF0) >> 4);

    if (header.is_nes2 != 2) {
        header.byte8.prgram_size = flags[8];

        header.byte9.frequency = @intCast(flags[9] & 1);

        header.byteA.one.frequency_2  = @intCast((flags[10] & 0x03) >> 0);
        header.byteA.one.hasnt_prgram = @intCast((flags[10] & 0x04) >> 2);
        header.byteA.one.bus_conflict = @intCast((flags[10] & 0x08) >> 3);
        return;
    }

    // is nes2.0
    header.byte8.two.mapper_hiest = @intCast((flags[8] & 0x0F) >> 0);
    header.byte8.two.submapper    = @intCast((flags[8] & 0xF0) >> 4);

    header.byte9.two.prgrom_hi = @intCast((flags[9] & 0x0F) >> 0);
    header.byte9.two.chrrom_hi = @intCast((flags[9] & 0xF0) >> 4);

    header.byteA.two.prgram_size =   @intCast((flags[10] & 0x0F) >> 0);
    header.byteA.two.prgnvram_size = @intCast((flags[10] & 0xF0) >> 4);
    
    header.chrram_size =   @intCast((flags[11] & 0x0F) >> 0);
    header.chrnvram_size = @intCast((flags[11] & 0xF0) >> 4);

    // to be later implemented
}

pub fn loadMem() !void {
    prgROM = try alloc.alloc(u8, @as(usize, header.prgrom_size) * 0x4000);
    chrROM = try alloc.alloc(u8, @as(usize, header.chrrom_size) * 0x2000);
    prgRAM = try alloc.alloc(u8, @as(usize, header.byte8.prgram_size) * 0x2000);
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
    var size: u32 = try calcSize(header.prgrom_size, header.byte9.two.prgrom_hi);
    prgROM = try alloc.alloc(u8, size * 0x4000);

    size = try calcSize(header.chrrom_size, header.byte9.two.chrrom_hi);
    chrROM = try alloc.alloc(u8, size * 0x2000);

    const shift: usize = 64;
    prgRAM = try alloc.alloc(u8, shift << header.byteA.two.prgram_size);
    chrRAM = try alloc.alloc(u8, shift << header.chrram_size);
    prgNVRAM = try alloc.alloc(u8, shift << header.byteA.two.prgnvram_size);
    chrNVRAM = try alloc.alloc(u8, shift << header.chrnvram_size);
}

pub fn read(index: u16) u8 {
    const mapper_num: u8 = @as(u8, header.mapper_lo + (header.mapper_hi << 8));
    return mapperReadTable[mapper_num](index);
}

const mapperInitTable: *const fn () void = .{
    @import("mappers/000_NROM.zig").init_000,
};

const mapperReadTable: *const fn (u16) u8 = .{
    @import("mappers/000_NROM.zig").read_000,
};

const mapperWriteTable: *const fn (u16, u8) void = .{
    @import("mappers/000_NROM.zig").write_000,
};