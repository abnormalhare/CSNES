const std = @import("std");

const NES = @import("nes.zig").NES;
const ALU = @import("alu.zig");
const OPTYPE = @import("opcode_types.zig");

// NOTE: All timings should have at least ONE call to the R_ or W_ wrappers

/// BRK
pub fn OP_00(this: *NES) void {
    if (this.ir == 0x00) {
        OPTYPE.BRK(this, 1);
    } else {
        OPTYPE.BRK(this, 0);
    }
}

/// [R] ORA (d,x)
pub fn OP_01(this: *NES) void {
    OPTYPE.IX_R(this);
    switch (this.timing) {
        else => {},
        5 => ALU.ORA(this),
    }
}

/// STP
pub fn OP_02(this: *NES) void {
    OPTYPE.STP(this, 0x02);
}

/// [M] SLO (d,x)
pub fn OP_03(this: *NES) void {
    OPTYPE.IX_M(this);
    switch (this.timing) {
        else => {},
        6 => {
            ALU.ASL(this);
            ALU.ORA(this);
        },
    }
}

/// [R] NOP d
pub fn OP_04(this: *NES) void {
    OPTYPE.D_R(this);
}

/// [R] ORA d
pub fn OP_05(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.ORA(this),
    }
}

/// [M] ASL d
pub fn OP_06(this: *NES) void {
    OPTYPE.D_M(this);
    switch (this.timing) {
        else => {},
        3 => ALU.ASL(this),
    }
}

/// [M] SLO d
pub fn OP_07(this: *NES) void {
    OPTYPE.D_M(this);
    switch (this.timing) {
        else => {},
        3 => {
            ALU.ASL(this);
            ALU.ORA(this);
        },
    }
}

/// PHP
pub fn OP_08(this: *NES) void {
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_getROM(this.pc),
        2 => { this.W_push(this.p.all | 0x30); },
    }
}

/// [R] ORA #i
pub fn OP_09(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.ORA(this),
    }
}

/// [M] ASL
pub fn OP_0A(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => ALU.ASLA(this),
    }
}

/// [R] ANC #i
pub fn OP_0B(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => {
            ALU.AND(this);
            this.p.flags.carry = this.p.flags.neg;
        },
    }
}

/// [R] NOP a
pub fn OP_0C(this: *NES) void {
    OPTYPE.A_R(this);
}

/// [R] ORA a
pub fn OP_0D(this: *NES) void {
    OPTYPE.A_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.ORA(this),
    }
}

/// [M] ASL a
pub fn OP_0E(this: *NES) void {
    OPTYPE.A_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.ASL(this),
    }
}

/// [M] SLO a
pub fn OP_0F(this: *NES) void {
    OPTYPE.A_M(this);
    switch (this.timing) {
        else => {},
        4 => {
            ALU.ASL(this);
            ALU.ORA(this);
        },
    }
}

/// BPL
pub fn OP_10(this: *NES) void {
    OPTYPE.RA(this, this.p.flags.neg == 0);
}

/// [R] ORA (d),y
pub fn OP_11(this: *NES) void {
    OPTYPE.IY_R(this);
    switch (this.timing) {
        else => {},
        4 => if (this.add.flags.carry == 0) { ALU.ORA(this); },
        5 => ALU.ORA(this),
    }
}

/// STP
pub fn OP_12(this: *NES) void {
    OPTYPE.STP(this, 0x12);
}

/// [M] SLO (d),y
pub fn OP_13(this: *NES) void {
    OPTYPE.IY_M(this);
    switch (this.timing) {
        else => {},
        6 => {
            ALU.ASL(this);
            ALU.ORA(this);
        },
    }
}

/// [R] NOP d,x
pub fn OP_14(this: *NES) void {
    OPTYPE.DX_R(this);
}

/// [R] ORA d,x
pub fn OP_15(this: *NES) void {
    switch (this.timing) {
        else => {},
        3 => ALU.ORA(this),
    }
}

/// [M] ASL d,x
pub fn OP_16(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.ASL(this),
    }
}

/// [M] SLO d,x
pub fn OP_17(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => {
            ALU.ASL(this);
            ALU.ORA(this);
        },
    }
}

/// CLC
pub fn OP_18(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => this.p.flags.carry = 0,
    }
}

/// [R] ORA a,y
pub fn OP_19(this: *NES) void {
    OPTYPE.AY_R(this);
    switch (this.timing) {
        else => {},
        3 => if (this.add.flags.carry == 0) { ALU.ORA(this); },
        4 => ALU.ORA(this),
    }
}

/// [R] NOP
pub fn OP_1A(this: *NES) void {
    OPTYPE.I(this);
}

/// [M] SLO a,y
pub fn OP_1B(this: *NES) void {
    OPTYPE.AY_M(this);
    switch (this.timing) {
        else => {},
        5 => {
            ALU.ASL(this);
            ALU.ORA(this);
        },
    }
}

/// [R] NOP a,x
pub fn OP_1C(this: *NES) void {
    OPTYPE.AX_R(this);
}

/// [R] ORA a,x
pub fn OP_1D(this: *NES) void {
    OPTYPE.AX_M(this);
    switch (this.timing) {
        else => {},
        3 => if (this.add.flags.carry == 0) { ALU.ORA(this); },
        4 => ALU.ORA(this),
    }
}

/// [M] ASL a,x
pub fn OP_1E(this: *NES) void {
    OPTYPE.AX_M(this);
    switch (this.timing) {
        else => {},
        5 => ALU.ASL(this),
    }
}

/// [M] SLO a,x
pub fn OP_1F(this: *NES) void {
    OPTYPE.AX_M(this);
    switch (this.timing) {
        else => {},
        5 => {
            ALU.ASL(this);
            ALU.ORA(this);
        },
    }
}

/// JSR a
pub fn OP_20(this: *NES) void {
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_readROM(),
        2 => this.setPC(this.data, 0),
        3 => this.W_push(@truncate(this.pc >> 8)),
        4 => this.W_push(@truncate(this.pc >> 0)),
        5 => {
            this.R_readROM();
            this.setPC(this.data, 1);
        }
    }
}

/// [R] AND (d,x)
pub fn OP_21(this: *NES) void {
    OPTYPE.IX_R(this);
    switch (this.timing) {
        else => {},
        5 => ALU.AND(this),
    }
}

/// STP
pub fn OP_22(this: *NES) void {
    OPTYPE.STP(this, 0x22);
}

/// [M] RLA (d,x)
pub fn OP_23(this: *NES) void {
    OPTYPE.IX_M(this);
    switch (this.timing) {
        else => {},
        6 => {
            ALU.ROL(this);
            ALU.AND(this);
        },
    }
}

/// [R] BIT d
pub fn OP_24(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.BIT(this),
    }
}

/// [R] AND d
fn OP_25(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.AND(this),
    }
}

/// [M] ROL d
fn OP_26(this: *NES) void {
    OPTYPE.D_M(this);
    switch (this.timing) {
        else => {},
        3 => ALU.ROL(this),
    }
}

/// [M] RLA d
fn OP_27(this: *NES) void {
    OPTYPE.D_M(this);
    switch (this.timing) {
        else => {},
        3 => {
            ALU.ROL(this);
            ALU.AND(this);
        },
    }
}

/// PLP
pub fn OP_28(this: *NES) void {
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_getROM(this.pc),
        2 => {},
        3 => { const fuckYouZig: u8 = 0x30; this.p.all = (this.p.all & fuckYouZig) | (this.R_pop() & ~fuckYouZig); }
    }
}

/// [R] AND #i
fn OP_29(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.AND(this),
    }
}

/// [R] ROL
fn OP_2A(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => ALU.ROLA(this),
    }
}

/// [R] ANC #i
fn OP_2B(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => {
            ALU.AND(this);
            ALU.LSRA(this);
        },
    }
}

/// [R] BIT a
pub fn OP_2C(this: *NES) void {
    OPTYPE.A_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.BIT(this),
    }
}

/// [R] AND a
pub fn OP_2D(this: *NES) void {
    OPTYPE.A_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.AND(this),
    }
}

/// [M] ROL a
pub fn OP_2E(this: *NES) void {
    OPTYPE.A_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.ROL(this),
    }
}

/// [M] RLA a
pub fn OP_2F(this: *NES) void {
    OPTYPE.A_M(this);
    switch (this.timing) {
        else => {},
        4 => {
            ALU.ROL(this);
            ALU.AND(this);
        },
    }
}

/// BMI
pub fn OP_30(this: *NES) void {
    OPTYPE.RA(this, this.p.flags.neg == 1);
}

/// [R] AND (d),y
pub fn OP_31(this: *NES) void {
    OPTYPE.IX_R(this);
    switch (this.timing) {
        else => {},
        4 => if (this.add.flags.carry == 0) { ALU.AND(this); },
        5 => ALU.AND(this),
    }
}

/// STP
pub fn OP_32(this: *NES) void {
    OPTYPE.STP(this, 0x32);
}

/// [M] RLA (d),y
pub fn OP_33(this: *NES) void {
    OPTYPE.IY_M(this);
    switch (this.timing) {
        else => {},
        6 => {
            ALU.ROL(this);
            ALU.AND(this);
        },
    }
}

/// [R] NOP d,x
pub fn OP_34(this: *NES) void {
    OPTYPE.DX_R(this);
}

/// [R] AND d,x
pub fn OP_35(this: *NES) void {
    OPTYPE.DX_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.AND(this),
    }
}

/// [M] ROL d,x
pub fn OP_36(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.ROL(this),
    }
}

/// [M] RLA d,x
pub fn OP_37(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => {
            ALU.ROL(this);
            ALU.AND(this);
        },
    }
}

/// SEC
pub fn OP_38(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => this.p.flags.carry = 1,
    }
}

/// [R] AND a,y
pub fn OP_39(this: *NES) void {
    OPTYPE.AY_R(this);
    switch (this.timing) {
        else => {},
        3 => if (this.add.flags.carry == 0) { ALU.AND(this); },
        4 => ALU.AND(this),
    }
}

/// [R] NOP
pub fn OP_3A(this: *NES) void {
    OPTYPE.I(this);
}

/// [M] RLA a,y
pub fn OP_3B(this: *NES) void {
    OPTYPE.AY_M(this);
    switch (this.timing) {
        else => {},
        5 => {
            ALU.ROL(this);
            ALU.AND(this);
        },
    }
}

/// [R] NOP a,x
pub fn OP_3C(this: *NES) void {
    OPTYPE.AX_R(this);
}

/// [R] AND a,x
pub fn OP_3D(this: *NES) void {
    OPTYPE.AX_R(this);
    switch (this.timing) {
        else => {},
        3 => if (this.add.flags.carry == 0) { ALU.AND(this); },
        4 => ALU.AND(this),
    }
}

/// [M] ROL a,x
pub fn OP_3E(this: *NES) void {
    OPTYPE.AX_M(this);
    switch (this.timing) {
        else => {},
        5 => ALU.ROL(this),
    }
}

/// [M] RLA a,x
pub fn OP_3F(this: *NES) void {
    OPTYPE.AX_M(this);
    switch (this.timing) {
        else => {},
        5 => {
            ALU.ROL(this);
            ALU.AND(this);
        },
    }
}

/// RTI
pub fn OP_40(this: *NES) void {
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_getROM(this.pc),
        2 => {},
        3 => { const fuckYouZig: u8 = 0x20; this.p.all = this.R_pop() & ~fuckYouZig; },
        4 => this.setPC(this.R_pop(), 1),
        5 => this.setPC(this.R_pop(), 0),
    }
}

/// [R] EOR (d,x)
pub fn OP_41(this: *NES) void {
    OPTYPE.IX_R(this);
    switch (this.timing) {
        else => {},
        5 => ALU.EOR(this),
    }
}

/// STP
pub fn OP_42(this: *NES) void {
    OPTYPE.STP(this, 0x42);
}

/// [M] SRE (d,x)
pub fn OP_43(this: *NES) void {
    OPTYPE.IX_M(this);
    switch (this.timing) {
        else => {},
        6 => {
            ALU.LSR(this);
            ALU.EOR(this);
        },
    }
}

/// [R] NOP d
pub fn OP_44(this: *NES) void {
    OPTYPE.D_R(this);
}

/// [R] EOR d
pub fn OP_45(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.EOR(this),
    }
}

/// [M] LSR d
pub fn OP_46(this: *NES) void {
    OPTYPE.D_M(this);
    switch (this.timing) {
        else => {},
        3 => ALU.LSR(this),
    }
}

/// [M] SRE d
pub fn OP_47(this: *NES) void {
    OPTYPE.D_M(this);
    switch (this.timing) {
        else => {},
        3 => {
            ALU.LSR(this);
            ALU.EOR(this);
        },
    }
}

/// PHA
pub fn OP_48(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => this.W_push(this.a),
    }
}

/// EOR #i
pub fn OP_49(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.EOR(this),
    }
}

/// [R] LSR
pub fn OP_4A(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => ALU.LSRA(this),
    }
}

/// [R] ALR #i
pub fn OP_4B(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => {
            ALU.AND(this);
            ALU.LSR(this);
        }
    }
}

/// JMP a
pub fn OP_4C(this: *NES) void {
    switch (this.timing) {
        else => this.resetTiming(),
        1 => { this.R_readROM(); this.setAB(this.data, 0); },
        2 => { this.R_readROM(); this.setAB(this.data, 1); },
        3 => { this.R_getROMWithAB(); this.data = this.ab.half.low; },
        4 => {
            this.setPC(this.data, 0);
            this.data = this.ab.half.high;
            this.setPC(this.data, 1);
        }
    }
}

/// [R] EOR a
pub fn OP_4D(this: *NES) void {
    OPTYPE.A_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.EOR(this),
    }
}

/// [M] LSR a
pub fn OP_4E(this: *NES) void {
    OPTYPE.A_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.LSR(this),
    }
}

/// [M] SRE a
pub fn OP_4F(this: *NES) void {
    OPTYPE.A_M(this);
    switch (this.timing) {
        else => {},
        4 => {
            ALU.LSR(this);
            ALU.EOR(this);
        },
    }
}

/// BVC
pub fn OP_50(this: *NES) void {
    OPTYPE.RA(this, this.p.flags.over == 0);
}

/// [R] EOR (d),y
pub fn OP_51(this: *NES) void {
    OPTYPE.IY_R(this);
    switch (this.timing) {
        else => {},
        4 => if (this.add.flags.carry == 0) { ALU.EOR(this); },
        5 => ALU.EOR(this),
    }
}

/// STP
pub fn OP_52(this: *NES) void {
    OPTYPE.STP(this, 0x52);
}

/// [M] SRE (d),y
pub fn OP_53(this: *NES) void {
    OPTYPE.IY_M(this);
    switch (this.timing) {
        else => {},
        6 => {
            ALU.LSR(this);
            ALU.EOR(this);
        },
    }
}

/// [R] NOP d,x
pub fn OP_54(this: *NES) void {
    OPTYPE.DX_R(this);
}

/// [R] EOR d,x
pub fn OP_55(this: *NES) void {
    OPTYPE.DX_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.EOR(this),
    }
}

/// [M] LSR d,x
pub fn OP_56(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.LSR(this),
    }
}

/// [M] SRE d,x
pub fn OP_57(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => {
            ALU.LSR(this);
            ALU.EOR(this);
        },
    }
}

/// CLI
pub fn OP_58(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => this.setI = false,
    }
}

/// [R] EOR a,y
pub fn OP_59(this: *NES) void {
    OPTYPE.AY_R(this);
    switch (this.timing) {
        else => {},
        3 => if (this.add.flags.carry == 0) { ALU.EOR(this); },
        4 => ALU.EOR(this),
    }
}

/// [R] NOP
pub fn OP_5A(this: *NES) void {
    OPTYPE.I(this);
}

/// [M] SRE a,y
pub fn OP_5B(this: *NES) void {
    OPTYPE.AY_M(this);
    switch (this.timing) {
        else => {},
        5 => {
            ALU.LSR(this);
            ALU.EOR(this);
        },
    }
}

/// [R] NOP a,x
pub fn OP_5C(this: *NES) void {
    OPTYPE.AX_R(this);
}

/// [R] EOR a,x
pub fn OP_5D(this: *NES) void {
    OPTYPE.AX_R(this);
    switch (this.timing) {
        else => {},
        3 => if (this.add.flags.carry == 0) { ALU.EOR(this); },
        4 => ALU.EOR(this),
    }
}

/// [M] LSR a,x
pub fn OP_5E(this: *NES) void {
    OPTYPE.AX_M(this);
    switch (this.timing) {
        else => {},
        5 => ALU.LSR(this),
    }
}

/// [M] SRE a,x
pub fn OP_5F(this: *NES) void {
    OPTYPE.AX_M(this);
    switch (this.timing) {
        else => {},
        5 => {
            ALU.LSR(this);
            ALU.EOR(this);
        },
    }
}

/// RTS
pub fn OP_60(this: *NES) void {
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_getROMWithD(),
        2 => {},
        3 => this.setPC(this.R_pop(), 0),
        4 => this.setPC(this.R_pop(), 1),
        5 => this.pc += 1,
    }
}

/// [R] ADC (d,x)
pub fn OP_61(this: *NES) void {
    OPTYPE.IX_R(this);
    switch (this.timing) {
        else => {},
        5 => ALU.ADC(this),
    }
}

/// STP
pub fn OP_62(this: *NES) void {
    OPTYPE.STP(this, 0x62);
}

/// [M] RRA (d,x)
pub fn OP_63(this: *NES) void {
    OPTYPE.IX_M(this);
    switch (this.timing) {
        else => {},
        6 => {
            ALU.ROR(this);
            ALU.ADC(this);
        },
    }
}

/// [R] NOP d
pub fn OP_64(this: *NES) void {
    OPTYPE.D_R(this);
}

/// [R] ADC d
pub fn OP_65(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.ADC(this),
    }
}

/// [M] ROR d
pub fn OP_66(this: *NES) void {
    OPTYPE.D_M(this);
    switch (this.timing) {
        else => {},
        3 => ALU.ROR(this),
    }
}

/// [M] RRA d
pub fn OP_67(this: *NES) void {
    OPTYPE.D_M(this);
    switch (this.timing) {
        else => {},
        3 => {
            ALU.ROR(this);
            ALU.ADC(this);
        },
    }
}

/// PLA
pub fn OP_68(this: *NES) void {
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_getROM(this.pc),
        2 => {},
        3 => {
            this.a = this.R_pop();

            this.p.flags.zero = @intFromBool(this.a == 0x00);
            this.p.flags.neg  = @intFromBool(this.a >= 0x80);
        },
    }
}

/// ADC #i
pub fn OP_69(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.ADC(this),
    }
}

//...

/// BVS
pub fn OP_70(this: *NES) void {
    OPTYPE.RA(this, this.p.flags.over == 1);
}

/// [R] ADC (d),y
pub fn OP_71(this: *NES) void {
    OPTYPE.IY_R(this);
    switch (this.timing) {
        else => {},
        4 => if (this.add.flags.carry == 0) { ALU.ADC(this); },
        5 => ALU.ADC(this),
    }
}

/// STP
pub fn OP_72(this: *NES) void {
    OPTYPE.STP(this, 0x72);
}

/// [M] RRA (d),y
pub fn OP_73(this: *NES) void {
    OPTYPE.IY_M(this);
    switch (this.timing) {
        else => {},
        6 => {
            ALU.ROR(this);
            ALU.ADC(this);
        },
    }
}

/// [R] NOP d,x
pub fn OP_74(this: *NES) void {
    OPTYPE.DX_R(this);
}

/// [R] ADC d,x
pub fn OP_75(this: *NES) void {
    OPTYPE.DX_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.ADC(this),
    }
}

/// [M] ROR d,x
pub fn OP_76(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.ROR(this),
    }
}

/// [M] RRA d,x
pub fn OP_77(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => {
            ALU.ROR(this);
            ALU.ADC(this);
        },
    }
}

/// SEI
pub fn OP_78(this: *NES) void {
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.setI = true,
    }
}

//...

/// [R] NOP #i
pub fn OP_80(this: *NES) void {
    OPTYPE.M(this);
}

/// [R] STA (d,x)
pub fn OP_81(this: *NES) void {
    OPTYPE.IX_W(this);
    switch (this.timing) {
        else => {},
        5 => this.W_writeROMVD(this.a),
    }
}

/// [R] NOP #i
pub fn OP_82(this: *NES) void {
    OPTYPE.M(this);
}

/// [W] STA (d,x)
pub fn OP_83(this: *NES) void {
    OPTYPE.IX_W(this);
    switch (this.timing) {
        else => {},
        5 => this.W_writeROMVD(this.a & this.x),
    }
}

/// [W] STY d
pub fn OP_84(this: *NES) void {
    OPTYPE.D_W(this);
    switch (this.timing) {
        else => {},
        2 => this.W_writeROMVD(this.y),
    }
}

/// [W] STA d
pub fn OP_85(this: *NES) void {
    OPTYPE.D_W(this);
    switch (this.timing) {
        else => {},
        2 => this.W_writeROMVD(this.a),
    }
}

/// [W] STX d
pub fn OP_86(this: *NES) void {
    OPTYPE.D_W(this);
    switch (this.timing) {
        else => {},
        2 => this.W_writeROMVD(this.x),
    }
}

/// [W] SAX d
pub fn OP_87(this: *NES) void {
    OPTYPE.D_W(this);
    switch (this.timing) {
        else => {},
        2 => this.W_writeROMVD(this.a & this.x),
    }
}

/// DEY
pub fn OP_88(this: *NES) void {
    switch (this.timing) {
        else => {},
        1 => {
            this.y -= 1;

            this.p.flags.zero = @intFromBool(this.y == 0x00);
            this.p.flags.neg  = @intFromBool(this.y >= 0x80);
        },
    }
}

/// BCC
pub fn OP_90(this: *NES) void {
    OPTYPE.RA(this, this.p.flags.carry == 0);
}

/// [R] STA (d),y
pub fn OP_91(this: *NES) void {
    OPTYPE.IY_R(this);
    switch (this.timing) {
        else => {},
        4 => if (this.add.flags.carry == 0) { this.W_writeROMVD(this.a); },
        5 => this.W_writeROMVD(this.a),
    }
}

/// STP
pub fn OP_92(this: *NES) void {
    OPTYPE.STP(this, 0x92);
}

// ...

/// [R] LDY #i
pub fn OP_A0(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.LDY(this),
    }
}

/// [R] LDA (d,x)
pub fn OP_A1(this: *NES) void {
    OPTYPE.IX_R(this);
    switch (this.timing) {
        else => {},
        5 => ALU.LDA(this),
    }
}

/// [R] LDX #i
pub fn OP_A2(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.LDX(this),
    }
}

/// [R] LAX (d,x)
pub fn OP_A3(this: *NES) void {
    OPTYPE.IX_R(this);
    switch (this.timing) {
        else => {},
        5 => {
            ALU.LDA(this);
            ALU.TAX(this);
        },
    }
}

/// [R] LDY d
pub fn OP_A4(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.LDY(this),
    }
}

/// [R] LDA d
pub fn OP_A5(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.LDA(this),
    }
}

/// [R] LDX d
pub fn OP_A6(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.LDX(this),
    }
}

/// [R] LAX d
pub fn OP_A7(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => {
            ALU.LDA(this);
            ALU.TAX(this);
        },
    }
}

/// [R] TAY
pub fn OP_A8(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => ALU.TAY(this),
    }
}

/// [R] LDA #i
pub fn OP_A9(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.LDA(this),
    }
}

//...

/// BCS
pub fn OP_B0(this: *NES) void {
    OPTYPE.RA(this, this.p.flags.carry == 1);
}

/// [R] LDA (d),y
pub fn OP_B1(this: *NES) void {
    OPTYPE.IY_R(this);
    switch (this.timing) {
        else => {},
        4 => if (this.add.flags.carry == 0) { ALU.LDA(this); },
        5 => ALU.LDA(this),
    }
}

/// STP
pub fn OP_B2(this: *NES) void {
    OPTYPE.STP(this, 0xB2);
}

/// [M] LAX (d),y
pub fn OP_B3(this: *NES) void {
    OPTYPE.IY_M(this);
    switch (this.timing) {
        else => {},
        6 => {
            ALU.LDA(this);
            ALU.TAX(this);
        },
    }
}

/// [R] LDY d,x
pub fn OP_B4(this: *NES) void {
    OPTYPE.DX_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.LDY(this),
    }
}

/// [R] LDA d,x
pub fn OP_B5(this: *NES) void {
    OPTYPE.DX_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.LDA(this),
    }
}

/// [R] LDX d,y
pub fn OP_B6(this: *NES) void {
    OPTYPE.DY_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.LDX(this),
    }
}

/// [R] LAX d,y
pub fn OP_B7(this: *NES) void {
    OPTYPE.DY_R(this);
    switch (this.timing) {
        else => {},
        3 => {
            ALU.LDA(this);
            ALU.TAX(this);
        },
    }
}

/// CLV
pub fn OP_B8(this: *NES) void {
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.p.flags.over = 0,
    }
}

//...

/// CPY #i
pub fn OP_C0(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.CPY(this),
    }
}

/// [R] CMP (d,x)
pub fn OP_C1(this: *NES) void {
    OPTYPE.IX_R(this);
    switch (this.timing) {
        else => {},
        5 => ALU.CMP(this),
    }
}

/// [R] NOP #i
pub fn OP_C2(this: *NES) void {
    OPTYPE.M(this);
}

/// [R] DCP (d,x)
pub fn OP_C3(this: *NES) void {
    OPTYPE.IX_R(this);
    switch (this.timing) {
        else => {},
        5 => {
            ALU.DEC(this);
            ALU.CMP(this);
        },
    }
}

/// [R] CPY d
pub fn OP_C4(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.CPY(this),
    }
}

/// [R] CMP d
pub fn OP_C5(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.CMP(this),
    }
}

/// [M] DEC d
pub fn OP_C6(this: *NES) void {
    OPTYPE.D_M(this);
    switch (this.timing) {
        else => {},
        3 => ALU.DEC(this),
    }
}

/// [M] DCP d
pub fn OP_C7(this: *NES) void {
    OPTYPE.D_M(this);
    switch (this.timing) {
        else => {},
        3 => {
            ALU.DEC(this);
            ALU.CMP(this);
        },
    }
}

/// INY
pub fn OP_C8(this: *NES) void {
    switch (this.timing) {
        else => this.resetTiming(),
        1 => {
            this.y += 1;

            this.p.flags.zero = @intFromBool(this.y == 0);
            this.p.flags.neg  = @intFromBool(this.y >= 0);
        },
    }
}

/// [R] CMP #i
pub fn OP_C9(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.CMP(this),
    }
}

/// DEX
pub fn OP_CA(this: *NES) void {
    switch (this.timing) {
        else => {},
        1 => {
            this.x -= 1;

            this.p.flags.zero = @intFromBool(this.x == 0x00);
            this.p.flags.neg  = @intFromBool(this.x >= 0x80);
        },
    }
}

//...

/// BNE
pub fn OP_D0(this: *NES) void {
    OPTYPE.RA(this, this.p.flags.zero == 0);
}

/// [R] CMP (d),y
pub fn OP_D1(this: *NES) void {
    OPTYPE.IY_R(this);
    switch (this.timing) {
        else => {},
        4 => if (this.add.flags.carry == 0) { ALU.CMP(this); },
        5 => ALU.CMP(this),
    }
}

/// STP
pub fn OP_D2(this: *NES) void {
    OPTYPE.STP(this, 0xD2);
}

/// [M] DCP (d),y
pub fn OP_D3(this: *NES) void {
    OPTYPE.IY_M(this);
    switch (this.timing) {
        else => {},
        6 => {
            ALU.DEC(this);
            ALU.CMP(this);
        },
    }
}

/// [R] NOP d,x
pub fn OP_D4(this: *NES) void {
    OPTYPE.DX_R(this);
}

/// [R] CMP d,x
pub fn OP_D5(this: *NES) void {
    OPTYPE.DX_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.CMP(this),
    }
}

/// [M] DEC d,x
pub fn OP_D6(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.DEC(this),
    }
}

/// [M] DCP d,x
pub fn OP_D7(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => {
            ALU.DEC(this);
            ALU.CMP(this);
        },
    }
}

/// CLD
pub fn OP_D8(this: *NES) void {
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.p.flags.dec = 0,
    }
}

//...

/// CPX #i
pub fn OP_E0(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.CPX(this),
    }
}

/// INX
pub fn OP_E8(this: *NES) void {
    switch (this.timing) {
        else => this.resetTiming(),
        1 => {
            this.x += 1;

            this.p.flags.zero = @intFromBool(this.x == 0);
            this.p.flags.neg  = @intFromBool(this.x >= 0);
        },
    }
}

/// SBC #i
pub fn OP_E9(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.SBC(this),
    }
}

/// NOP
pub fn OP_EA(this: *NES) void {
    OPTYPE.I(this);
}

//...

/// BEQ
pub fn OP_F0(this: *NES) void {
    OPTYPE.RA(this, this.p.flags.zero == 1);
}

/// [R] SBC (d),y
pub fn OP_F1(this: *NES) void {
    OPTYPE.IY_R(this);
    switch (this.timing) {
        else => {},
        4 => if (this.add.flags.carry == 0) { ALU.SBC(this); },
        5 => ALU.SBC(this),
    }
}

/// STP
pub fn OP_F2(this: *NES) void {
    OPTYPE.STP(this, 0xF2);
}

/// [M] ISC (d),y
pub fn OP_F3(this: *NES) void {
    OPTYPE.IY_M(this);
    switch (this.timing) {
        else => {},
        6 => {
            ALU.INC(this);
            ALU.SBC(this);
        },
    }
}

/// [R] NOP d,x
pub fn OP_F4(this: *NES) void {
    OPTYPE.DX_R(this);
}

/// [R] SBC d,x
pub fn OP_F5(this: *NES) void {
    OPTYPE.DX_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.SBC(this),
    }
}

/// [M] INC d,x
pub fn OP_F6(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.INC(this),
    }
}

/// [M] ISC d,x
pub fn OP_F7(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => {
            ALU.INC(this);
            ALU.SBC(this);
        },
    }
}

pub fn OP_F8(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => this.p.flags.dec = 1,
    }
}


pub const opTable = [_]*const fn(*NES) void{
    OP_00, OP_01, OP_02, OP_03, OP_04, OP_05, OP_06, OP_07, OP_08, OP_09, OP_0A, OP_0B, OP_0C, OP_0D, OP_0E, OP_0F,
    OP_10, OP_11, OP_12, OP_13, OP_14, OP_15, OP_16, OP_17, OP_18, OP_19, OP_1A, OP_1B, OP_1C, OP_1D, OP_1E, OP_1F,
    OP_20, OP_21, OP_22, OP_23, OP_24, OP_25, OP_26, OP_27, OP_28, OP_29, OP_2A, OP_2B, OP_2C, OP_2D, OP_2E, OP_2F,
    OP_30, OP_31, OP_32, OP_33, OP_34, OP_35, OP_36, OP_37, OP_38, OP_39, OP_3A, OP_3B, OP_3C, OP_3D, OP_3E, OP_3F,
    OP_40, OP_41, OP_42, OP_43, OP_44, OP_45, OP_46, OP_47, OP_48, OP_49, OP_4A, OP_4B, OP_4C, OP_4D, OP_4E, OP_4F,
    OP_50, OP_51, OP_52, OP_53, OP_54, OP_55, OP_56, OP_57, OP_58, OP_59, OP_5A, OP_5B, OP_5C, OP_5D, OP_5E, OP_5F,
    OP_60, OP_61, OP_62, OP_63, OP_64, OP_65, OP_66, OP_67, OP_68, OP_69, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02,
    OP_70, OP_71, OP_72, OP_73, OP_74, OP_75, OP_76, OP_77, OP_78, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02,
    OP_80, OP_81, OP_82, OP_83, OP_84, OP_85, OP_86, OP_87, OP_88, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02,
    OP_90, OP_91, OP_92, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02,
    OP_A0, OP_A1, OP_A2, OP_A3, OP_A4, OP_A5, OP_A6, OP_A7, OP_A8, OP_A9, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02,
    OP_B0, OP_B1, OP_B2, OP_B3, OP_B4, OP_B5, OP_B6, OP_B7, OP_B8, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02,
    OP_C0, OP_C1, OP_C2, OP_C3, OP_C4, OP_C5, OP_C6, OP_C7, OP_C8, OP_C9, OP_CA, OP_02, OP_02, OP_02, OP_02, OP_02,
    OP_D0, OP_D1, OP_D2, OP_D3, OP_D4, OP_D5, OP_D6, OP_D7, OP_D8, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02,
    OP_E0, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02, OP_E8, OP_E9, OP_EA, OP_02, OP_02, OP_02, OP_02, OP_02,
    OP_F0, OP_F1, OP_F2, OP_F3, OP_F4, OP_F5, OP_F6, OP_F7, OP_F8, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02,
};