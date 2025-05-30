const std = @import("std");
const alloc = @import("global").alloc;

const def = @import("defines.zig");
const mapper = @import("mapper.zig");
const OPTYPE = @import("opcode_types.zig");

const read = @import("rw.zig").read;
const write = @import("rw.zig").write;
const opTable = @import("opcodes.zig").opTable;


var debugLength: i32 = 0;
var debugOut: std.fs.File = undefined;

pub const NES = struct {
    // base regs
    a: u8, x: u8, y: u8,
    pc: u16, sp: u8,
    data: u8,
    p: def.Status,
    setI: bool,

    // internal logic
    ir: u8,
    timing: u4,
    
    // only simulated when needed
    pcs: def.addr_bus, ab: def.addr_bus,
    add: def.Status,

    // External Connections
    RAM: [0x800]u8,
    SRAM: [0x2000]u8,
    ppu: def.PPU,

    // External Pins
    irq: u1,

    // emulation information
    cycleCount: u64,
    jam: u8,

    // INITIALIZATION FUNCTIONS //
    fn loadRegs(this: *NES) void {
        this.a = 0;
        this.x = 0;
        this.y = 0;

        this.pc = 0xC000; // 0xFFFC;
        this.ab.full = this.pc;
        this.sp = 0xFD;

        this.ir = 0;
        this.timing = 0;
        
        this.p.all = 0b00100100;
        this.setI = false;

        this.RAM = [_]u8{0} ** 0x800;
        this.SRAM = [_]u8{0} ** 0x2000;

        this.ppu.PPUCTRL = 0;
        this.ppu.PPUMASK = 0;
        this.ppu.PPUSTATUS = 0;
        this.ppu.OAMADDR = 0;
        this.ppu.isEvenFrame = false;
        this.ppu.scanline = 0;
        this.ppu.dot = 0;

        this.cycleCount = 0;
        this.jam = 0;
    }

    fn initFiles(this: *NES, filename: []const u8) ?std.fs.File {
        const file: std.fs.File = std.fs.cwd().openFile(filename, .{}) catch |err| switch (err) {
            error.FileNotFound => {
                std.debug.print("ERROR: The file '{s}' cannot be found.", .{filename});
                return null;
            },
            error.AccessDenied => {
                std.debug.print("ERROR: Cannot open '{s}': Access Denied.", .{filename});
                return null;
            },
            else => {
                std.debug.print("ERROR: Unhandled error '{any}' for file '{s}'.", .{err, filename});
                return null;
            }
        };
        debugOut = std.fs.cwd().createFile("output.txt", .{}) catch |err| switch (err) {
            error.AccessDenied => {
                std.debug.print("ERROR: Cannot open '{s}': Access Denied.", .{filename});
                return null;
            },
            else => {
                std.debug.print("ERROR: Unhandled error '{any}' for file '{s}'.", .{err, filename});
                return null;
            }
        };
        _ = this;

        return file;
    }

    pub fn init(filename: []const u8) !?*NES {
        const nes: *NES = try alloc.create(NES);
        const file = nes.initFiles(filename);

        nes.loadRegs();

        const validNES: bool = try mapper.initMapper(file.?);
        if (!validNES) return null;

        const stat = try file.?.stat();
        const fullFile: []u8 = try alloc.alloc(u8,stat.size - 0x10);
        _ = try file.?.read(fullFile);

        return nes;
    }

    pub fn run(this: *NES) void {
        switch (this.timing) {
            0 => {
                std.debug.print("\n PC: {X:0>2} | ", .{this.pc});
                this.ab.full = this.pc;
                this.pc += 1;
                this.R_getROMWithAB();
                this.prtOp();
                this.ir = this.data;
            },
            else => {
                if (this.p.flags.intd) {
                    opTable[0x00](this);
                    return;
                }

                opTable[this.ir](this);
                
                if (this.timing == 0) this.run();
            }
        }
    }

    fn prtOp(this: *NES) void {
        std.debug.print("0x{X:0>2} ", .{this.data});
    }

    pub fn resetTiming(this: *NES) void {
        this.timing = 0;
    }

    pub fn checkIRQ(this: *NES) void {
        this.p.flags.intd = this.setI;
    }

    // R/W WRAPPER FUNCTIONS //
    pub fn R_getROM(this: *NES, index: u16) void {
        this.data = read(this, index);
    }

    pub fn R_getOpcode(this: *NES, index: u16) void {
        this.data = read(this, index);
        this.ir = this.data;
    }

    pub fn R_readROM(this: *NES) void {
        this.R_getROM(this.pc);
        this.pc, _ = @addWithOverflow(this.pc, 1);
        this.prtOp();
    }

    pub fn R_readOpcode(this: *NES) void {
        this.R_getOpcode(this.pc);
        this.pc, _ = @addWithOverflow(this.pc, 1);
    }

    pub fn R_getROMWithAB(this: *NES) void {
        this.R_getROM(this.ab.full);
    }

    pub fn R_getROMWithD(this: *NES) void {
        this.ab.half.low = this.data;
        this.R_getROM(this.ab.half.low);
    }

    pub fn W_setROM(this: *NES, index: u16, data: u8) void {
        write(this, index, data);
    }

    pub fn W_writeROM(this: *NES) void {
        this.W_setROM(this.ab.full, this.data);
    }

    pub fn W_writeROMD(this: *NES) void {
        this.W_setROM(this.ab.half.low, this.data);
    }

    pub fn W_push(this: *NES, data: u8) void {
        write(this, 0x100 + @as(u16, this.sp), data);
        this.sp, _ = @subWithOverflow(this.sp, 1);
    }

    pub fn W_pop(this: *NES) u8 {
        this.sp, _ = @addWithOverflow(this.sp, 1);
        return read(this, 0x100 + @as(u16, this.sp));
    }

    pub fn setPC(this: *NES, data: u8, stage: u1) void {
        switch (stage) {
            0 => {
                this.pcs.half.high = data;
            },
            1 => {
                this.pcs.half.low = data;
                this.pc = this.pcs.full;
            }
        }
    }

    pub fn setAB(this: *NES, data: u8, stage: u1) void {
        switch (stage) {
            0 => {
                this.ab.half.high = data;
            },
            1 => {
                this.ab.half.low = data;
            }
        }
    }

    pub fn R_setPCIndirect(this: *NES, addr: u16, stage: u1) void {
        this.setPC(read(this, addr), stage);
    }

    pub fn R_setABIndirect(this: *NES, addr: u16, stage: u1) void {
        this.setAB(read(this, addr), stage);
    }

    pub fn ABAdd(this: *NES, val: u8, stage: u1) void {
        switch (stage) {
            0 => {
                this.ab.half.low, this.add.flags.carry = @addWithOverflow(this.ab.half.low, val);
            },
            1 => {
                this.ab.half.high, _ = @addWithOverflow(this.ab.half.high, this.add.flags.carry);
            }
        }
    }

    pub fn addData(this: *NES, val: u8) void {
        this.data, _ = @addWithOverflow(this.data, val);
    }
};