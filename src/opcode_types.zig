const NES = @import("nes.zig").NES;

pub fn STP(this: *NES, code: u8) void {
    this.jam = code;
}

pub fn BRK(this: *NES, brk: u1) void {
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => { this.W_push(@truncate(this.pc >> 0)); this.p.flags.brk = brk; },
        3 => this.W_push(@truncate(this.pc >> 8)),
        4 => this.W_push(this.p.all),
        5 => this.R_setPCIndirect(0xFFFE, 0),
        6 => this.R_setPCIndirect(0xFFFF, 1),
    }
}

/// accumulator / implied addressing
pub fn I(this: *NES) void {
    this.checkIRQ(1);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_getROMWithPC(),
    }
}

/// immediate addressing
pub fn M(this: *NES) void {
    this.checkIRQ(1);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
    }
}

/// absolute addressing, read
pub fn A_R(this: *NES) void {
    this.checkIRQ(3);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => { this.setAB(this.data, 0); this.R_readROM(); },
        3 => { this.setAB(this.data, 1); this.R_getROMWithAB(); },
    }
}

/// absolute addressing, rmw
pub fn A_M(this: *NES) void {
    this.checkIRQ(5);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => { this.setAB(this.data, 0); this.R_readROM(); },
        3 => { this.setAB(this.data, 1); this.R_getROMWithAB(); },
        4 => this.W_writeROM(),
        5 => this.W_writeROM(),
    }
}

/// absolute addressing, write
/// NOTE: you must write manually
pub fn A_W(this: *NES) void {
    this.checkIRQ(3);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => { this.setAB(this.data, 0); this.R_readROM(); },
        3 => this.setAB(this.data, 1),
    }
}

/// zero page addressing, read
pub fn D_R(this: *NES) void {
    this.checkIRQ(2);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => this.R_getROMWithD(),
    }
}

/// zero page addressing, rmw
pub fn D_M(this: *NES) void {
    this.checkIRQ(4);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => this.R_getROMWithD(),
        3 => this.W_writeROMD(),
        4 => this.W_writeROMD(),
    }
}

/// zero page addressing, write  
/// NOTE: you must write manually
pub fn D_W(this: *NES) void {
    this.checkIRQ(2);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => this.setAB(this.data, 0),
    }
}

/// zero page indexed addressing, read
pub fn DX_R(this: *NES) void {
    this.checkIRQ(3);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => this.addData(this.x),
        3 => this.R_getROMWithD(),
    }
}

/// zero page indexed addressing, rmw
pub fn DX_M(this: *NES) void {
    this.checkIRQ(5);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => this.addData(this.x),
        3 => this.R_getROMWithD(),
        4 => this.W_writeROMD(),
        5 => this.W_writeROMD(),
    }
}

/// zero page indexed addressing, write
/// NOTE: you must write manually
pub fn DX_W(this: *NES) void {
    this.checkIRQ(3);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => this.addData(this.x),
        3 => this.setAB(this.data, 0),
    }
}

/// zero page indexed addressing, read
pub fn DY_R(this: *NES) void {
    this.checkIRQ(3);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => this.addData(this.y),
        3 => this.R_getROMWithD(),
    }
}

/// zero page indexed addressing, rmw
pub fn DY_M(this: *NES) void {
    this.checkIRQ(5);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => this.addData(this.y),
        3 => this.R_getROMWithD(),
        4 => this.W_writeROMD(),
        5 => this.W_writeROMD(),
    }
}

/// zero page indexed addressing, write
/// NOTE: you must write manually
pub fn DY_W(this: *NES) void {
    this.checkIRQ(3);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => this.addData(this.y),
        3 => this.setAB(this.data, 0),
    }
}

/// absolute indexed x addressing, read
pub fn AX_R(this: *NES) void {
    this.checkIRQ(4);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => { this.setAB(this.data, 0); this.R_readROM(); },
        3 => {
            this.setAB(this.data, 1);
            this.ABAdd(this.x, 0);

            this.R_getROMWithAB();
            if (this.addF.flags.carry == 0) {
                this.timing = 0xE;
            }
        },
        4 => { this.ABAdd(this.x, 1); this.R_getROMWithAB(); },
    }
}

/// absolute indexed x addressing, rmw
pub fn AX_M(this: *NES) void {
    this.checkIRQ(6);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => { this.setAB(this.data, 0); this.R_readROM(); },
        3 => { this.setAB(this.data, 1); this.ABAdd(this.x, 0); this.R_getROMWithAB(); },
        4 => { this.ABAdd(this.x, 1); this.R_getROMWithAB(); },
        5 => this.W_writeROM(),
        6 => this.W_writeROM(),
    }
}

/// absolute indexed x addressing, write
/// NOTE: you must write manually
pub fn AX_W(this: *NES) void {
    this.checkIRQ(4);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => { this.setAB(this.data, 0); this.R_readROM(); },
        3 => { this.setAB(this.data, 1); this.ABAdd(this.x, 0); this.R_getROMWithAB(); },
        4 => this.ABAdd(this.x, 1),
    }
}

/// absolute indexed y addressing, read
pub fn AY_R(this: *NES) void {
    this.checkIRQ(4);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => { this.setAB(this.data, 0); this.R_readROM(); },
        3 => {
            this.setAB(this.data, 1);
            this.ABAdd(this.y, 0); 

            this.R_getROMWithAB();
            if (this.addF.flags.carry == 0) {
                this.timing = 0xE;
            }
        },
        4 => { this.ABAdd(this.y, 1); this.R_getROMWithAB(); },
    }
}

/// absolute indexed y addressing, rmw
pub fn AY_M(this: *NES) void {
    this.checkIRQ(6);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => { this.setAB(this.data, 0); this.R_readROM(); },
        3 => { this.setAB(this.data, 1); this.ABAdd(this.y, 0); this.R_getROMWithAB(); },
        4 => { this.ABAdd(this.y, 1); this.R_getROMWithAB(); },
        5 => this.W_writeROM(),
        6 => this.W_writeROM(),
    }
}

/// absolute indexed y addressing, write
/// NOTE: you must write manually
pub fn AY_W(this: *NES) void {
    this.checkIRQ(4);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => { this.setAB(this.data, 0); this.R_readROM(); },
        3 => { this.setAB(this.data, 1); this.ABAdd(this.y, 0); this.R_getROMWithAB(); },
        4 => this.ABAdd(this.y, 1),
    }
}

/// relative addressing (branches)
pub fn RA(this: *NES, check: bool) void {
    this.checkIRQ(3);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => {
            this.R_readROM();
            
            if (!check) { this.timing = 0xE; }
        },
        2 => {
            const pc_lo: u8 = @truncate(this.pc);
            var addr_lo: u8 = undefined;
            if (this.data < 0x80) {
                addr_lo, this.addF.flags.carry = @addWithOverflow(pc_lo, this.data);
                this.addF.flags.over = 0;
            } else {
                addr_lo, this.addF.flags.carry = @subWithOverflow(pc_lo, 255 - this.data + 1);
                this.addF.flags.over = 1;
            }
            this.setPC(addr_lo, 0);
            this.R_getROMWithPC();
            
            if (this.addF.flags.carry != 0) return;

            this.setPC(@truncate((this.pc >> 8)), 1);
            this.timing = 0xE;
        },
        3 => {
            if (this.addF.flags.over == 0) {
                this.setPC(@truncate((this.pc >> 8) + this.addF.flags.carry), 1);
            } else {
                this.setPC(@truncate((this.pc >> 8) - this.addF.flags.carry), 1);
            }
            this.R_getROMWithPC();
        },
    }
}

/// indexed indirect addressing, read
pub fn IX_R(this: *NES) void {
    this.checkIRQ(5);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => this.addData(this.x),
        3 => { this.add = this.data; this.R_getROMWithD(); },
        4 => { this.setABIndirect(this.data, 0); this.addData(1); this.R_getROMWithD(); },
        5 => { this.setABIndirect(this.data, 1); this.R_getROMWithAB(); },
    }
}

/// indexed indirect addressing, rmw
pub fn IX_M(this: *NES) void {
    this.checkIRQ(7);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => this.addData(this.x),
        3 => { this.add = this.data; this.R_getROMWithD(); },
        4 => { this.setABIndirect(this.data, 0); this.addData(1); this.R_getROMWithD(); },
        5 => { this.setABIndirect(this.data, 1); this.R_getROMWithAB(); },
        6 => this.W_writeROM(),
        7 => this.W_writeROM(),
    }
}

/// indexed indirect addressing, write
/// NOTE: you must write manually
pub fn IX_W(this: *NES) void {
    this.checkIRQ(5);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => this.addData(this.x),
        3 => { this.add = this.data; this.R_getROMWithD(); },
        4 => { this.setABIndirect(this.data, 0); this.addData(1); this.R_getROMWithD(); },
        5 => this.setABIndirect(this.data, 1),
    }
}

const std = @import("std");

/// indirect indexed addressing, read
pub fn IY_R(this: *NES) void {
    this.checkIRQ(5);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => { this.add = this.data; this.R_getROMWithD(); },
        3 => { this.setABIndirect(this.data, 0); this.addData(1); this.R_getROMWithD(); },
        4 => {
            this.setABIndirect(this.data, 1);
            this.ABAdd(this.y, 0);

            this.R_getROMWithAB();

            if (this.addF.flags.carry == 0) {
                this.timing = 0xE;
            }
        },
        5 => { this.ABAdd(this.y, 1); this.R_getROMWithAB(); },
    }
}

/// indirect indexed addressing, rmw
pub fn IY_M(this: *NES) void {
    this.checkIRQ(7);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => { this.add = this.data; this.R_getROMWithD(); },
        3 => { this.setABIndirect(this.data, 0); this.addData(1); this.R_getROMWithD(); },
        4 => { this.setABIndirect(this.data, 1); this.ABAdd(this.y, 0); this.R_getROMWithAB(); },
        5 => { this.ABAdd(this.y, 1); this.R_getROMWithAB(); },
        6 => this.W_writeROM(),
        7 => this.W_writeROM(),
    }
}

/// indirect indexed addressing, write
/// NOTE: you must write manually
pub fn IY_W(this: *NES) void {
    this.checkIRQ(5);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => { this.add = this.data; this.R_getROMWithD(); },
        3 => { this.setABIndirect(this.data, 0); this.addData(1); this.R_getROMWithD(); },
        4 => { this.setABIndirect(this.data, 1); this.ABAdd(this.y, 0); this.R_getROMWithAB(); },
        5 => { this.ABAdd(this.y, 1); this.R_getROMWithAB(); },
    }
}