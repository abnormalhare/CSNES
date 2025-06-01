const std = @import("std");
const alloc = @import("global").alloc;

const def = @import("defines.zig");
const PPU = @import("ppu.zig").PPU;
const mapper = @import("mapper.zig");
const OPTYPE = @import("opcode_types.zig");

const read = @import("rw.zig").read;
const write = @import("rw.zig").write;
const opTable = @import("opcodes.zig").opTable;

pub const NES = struct {
    // base regs
    a: u8,
    x: u8,
    y: u8,
    pc: u16,
    ab: def.addr_bus,
    sp: u8,
    data: u8,
    p: def.Status,
    setI: bool,

    // internal logic
    ir: u8,
    timing: u4,

    // only simulated when needed
    pcs: def.addr_bus,
    add: u8,
    addF: def.Status,

    // External Connections
    RAM: [0x800]u8,
    SRAM: [0x2000]u8,
    ppu: PPU,

    // External Pins
    irq: u1,

    // emulation information
    cycleCount: u64,
    jam: u8,

    // debug info
    cnt: u2,

    // INITIALIZATION FUNCTIONS //
    fn loadRegs(this: *NES) void {
        this.a = 0;
        this.x = 0;
        this.y = 0;

        this.pc = 0xC000;
        this.ab.full = this.pc;
        this.sp = 0xFD;

        this.ir = 0;
        this.timing = 0;

        this.p.all = 0b00100100;

        this.setI = true;
        this.irq = 1;

        this.RAM = [_]u8{0} ** 0x800;
        this.SRAM = [_]u8{0} ** 0x2000;

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
                std.debug.print("ERROR: Unhandled error '{any}' for file '{s}'.", .{ err, filename });
                return null;
            },
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
        const fullFile: []u8 = try alloc.alloc(u8, stat.size - 0x10);
        _ = try file.?.read(fullFile);

        try nes.ppu.init(nes);

        return nes;
    }

    fn prtEnd(this: *NES) void {
        const str: []const u8 = switch (this.cnt) {
            0 => "               ",
            1 => "            ",
            2 => "         ",
            3 => "      ",
        };
        std.debug.print("{s}| A:{X:0>2} X:{X:0>2} Y:{X:0>2} P:{X:0>2} | ", .{ str, this.a, this.x, this.y, this.p.all });

        const sp: u16 = @as(u16, this.sp) + 0x100;
        std.debug.print("0x{X:0>2}:{X:0>2} {X:0>2} {X:0>2} {X:0>2} | RAM:{X:0>2} {X:0>2} {X:0>2} {X:0>2}\n{X:0>4} | ", .{ this.sp, this.RAM[sp + 2], this.RAM[sp + 1], this.RAM[sp], this.RAM[sp - 1], this.RAM[0], this.RAM[0x1], this.RAM[2], this.RAM[3], this.pc });
        this.cnt = 0;

        // if (this.pc == 0xDB7B) {
        //     this.zpDump();
        // }
    }

    fn zpDump(this: *NES) void {
        const val: u16 = 0x180;
        std.debug.print("\n", .{});
        var i: u16 = val;
        while (i < val + 0x100) {
            std.debug.print("{X:0>2} | {X:0>2}{X:0>2}{X:0>2}{X:0>2}{X:0>2}{X:0>2}{X:0>2}{X:0>2}\n", .{ i, this.RAM[i], this.RAM[i + 1], this.RAM[i + 2], this.RAM[i + 3], this.RAM[i + 4], this.RAM[i + 5], this.RAM[i + 6], this.RAM[i + 7] });
            i += 8;
        }
    }

    pub fn startPC(this: *NES) void {
        this.pc = @as(u16, read(this, this.pc)) + (@as(u16, read(this, this.pc + 1)) << 8);
    }

    pub fn run(this: *NES) void {
        if (this.timing == 0) {
            this.prtEnd();

            this.ab.full = this.pc;
            this.pc += 1;
            this.R_getROMWithAB();

            this.prtOp();

            this.ir = this.data;
            return;
        }

        if (this.p.flags.intd == 0 and this.irq == 1) {
            opTable[0x00](this);
            return;
        }

        opTable[this.ir](this);

        if (this.timing == 0) this.run();
    }

    fn prtOp(this: *NES) void {
        std.debug.print("{X:0>2} ", .{this.data});
        this.cnt += 1;
    }

    pub fn resetTiming(this: *NES) void {
        this.timing = 0;
    }

    pub fn checkIRQ(this: *NES, time: u4) void {
        if (this.timing == time) {
            this.p.flags.intd = @intFromBool(this.setI);
        }
    }

    // R/W WRAPPER FUNCTIONS //

    /// sets data to the value at the given address
    fn R_getROM(this: *NES, index: u16) void {
        this.data = read(this, index);
    }

    /// sets both data and ir to the value at the given address
    fn R_getOpcode(this: *NES, index: u16) void {
        this.data = read(this, index);
        this.ir = this.data;
    }

    /// reads the value at PC and increments PC by 1
    pub fn R_readROM(this: *NES) void {
        this.R_getROM(this.pc);
        this.pc, _ = @addWithOverflow(this.pc, 1);
        this.prtOp();
    }

    /// sets data to the value at address pins
    pub fn R_getROMWithAB(this: *NES) void {
        this.R_getROM(this.ab.full);
    }

    /// puts the current data into the lower address and gets data using only that
    pub fn R_getROMWithD(this: *NES) void {
        this.ab.half.low = this.data;
        this.R_getROM(this.ab.half.low);
    }

    /// basic instruction for read calls that do not increment the PC
    pub fn R_getROMWithPC(this: *NES) void {
        this.R_getROM(this.pc);
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

    pub fn W_writeROMV(this: *NES, data: u8) void {
        this.W_setROM(this.ab.full, data);
    }

    pub fn W_writeROMVD(this: *NES, data: u8) void {
        this.W_setROM(this.ab.half.low, data);
    }

    pub fn W_push(this: *NES, data: u8) void {
        write(this, 0x100 + @as(u16, this.sp), data);
        this.sp, _ = @subWithOverflow(this.sp, 1);
    }

    pub fn R_pop(this: *NES) u8 {
        this.sp, _ = @addWithOverflow(this.sp, 1);
        return read(this, 0x100 + @as(u16, this.sp));
    }

    pub fn setPC(this: *NES, data: u8, stage: u1) void {
        switch (stage) {
            0 => {
                this.pcs.half.low = data;
            },
            1 => {
                this.pcs.half.high = data;
                this.pc = this.pcs.full;
            },
        }
    }

    pub fn setAB(this: *NES, data: u8, stage: u1) void {
        switch (stage) {
            0 => {
                this.ab.half.low = data;
            },
            1 => {
                this.ab.half.high = data;
            },
        }
    }

    pub fn setABIndirect(this: *NES, data: u8, stage: u1) void {
        switch (stage) {
            0 => {
                this.ab.half.low = data;
                this.data = this.add;
                this.add = this.ab.half.low;
            },
            1 => {
                this.ab.half.low = this.add;
                this.ab.half.high = data;
            },
        }
    }

    pub fn R_setPCIndirect(this: *NES, addr: u16, stage: u1) void {
        this.setPC(read(this, addr), stage);
    }

    pub fn ABAdd(this: *NES, val: u8, stage: u1) void {
        switch (stage) {
            0 => {
                this.ab.half.low, this.addF.flags.carry = @addWithOverflow(this.ab.half.low, val);
            },
            1 => {
                this.ab.half.high, _ = @addWithOverflow(this.ab.half.high, this.addF.flags.carry);
            },
        }
    }

    pub fn addData(this: *NES, val: u8) void {
        this.data, _ = @addWithOverflow(this.data, val);
    }
};
