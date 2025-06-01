const std = @import("std");

const NES = @import("nes.zig").NES;
const ALU = @import("alu.zig");
const OPTYPE = @import("opcode_types.zig");

// NOTE: All timings should have at least ONE call to the R_ or W_ wrappers

/// BRK
fn OP_00(this: *NES) void {
    if (this.ir == 0x00) {
        OPTYPE.BRK(this, 1);
    } else {
        OPTYPE.BRK(this, 0);
    }
}

/// [R] ORA (d,x)
fn OP_01(this: *NES) void {
    OPTYPE.IX_R(this);
    switch (this.timing) {
        else => {},
        5 => ALU.ORA(this),
    }
}

/// STP
fn OP_02(this: *NES) void {
    OPTYPE.STP(this, 0x02);
}

/// [M] SLO (d,x)
fn OP_03(this: *NES) void {
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
fn OP_04(this: *NES) void {
    OPTYPE.D_R(this);
}

/// [R] ORA d
fn OP_05(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.ORA(this),
    }
}

/// [M] ASL d
fn OP_06(this: *NES) void {
    OPTYPE.D_M(this);
    switch (this.timing) {
        else => {},
        3 => ALU.ASL(this),
    }
}

/// [M] SLO d
fn OP_07(this: *NES) void {
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
fn OP_08(this: *NES) void {
    this.checkIRQ(2);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_getROMWithPC(),
        2 => { this.W_push(this.p.all | 0x30); },
    }
}

/// [R] ORA #i
fn OP_09(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.ORA(this),
    }
}

/// [M] ASL
fn OP_0A(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => ALU.ASLA(this),
    }
}

/// [R] ANC #i
fn OP_0B(this: *NES) void {
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
fn OP_0C(this: *NES) void {
    OPTYPE.A_R(this);
}

/// [R] ORA a
fn OP_0D(this: *NES) void {
    OPTYPE.A_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.ORA(this),
    }
}

/// [M] ASL a
fn OP_0E(this: *NES) void {
    OPTYPE.A_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.ASL(this),
    }
}

/// [M] SLO a
fn OP_0F(this: *NES) void {
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
fn OP_10(this: *NES) void {
    OPTYPE.RA(this, this.p.flags.neg == 0);
}

/// [R] ORA (d),y
fn OP_11(this: *NES) void {
    OPTYPE.IY_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.ORA(this); },
        5 => ALU.ORA(this),
    }
}

/// STP
fn OP_12(this: *NES) void {
    OPTYPE.STP(this, 0x12);
}

/// [M] SLO (d),y
fn OP_13(this: *NES) void {
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
fn OP_14(this: *NES) void {
    OPTYPE.DX_R(this);
}

/// [R] ORA d,x
fn OP_15(this: *NES) void {
    switch (this.timing) {
        else => {},
        3 => ALU.ORA(this),
    }
}

/// [M] ASL d,x
fn OP_16(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.ASL(this),
    }
}

/// [M] SLO d,x
fn OP_17(this: *NES) void {
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
fn OP_18(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => this.p.flags.carry = 0,
    }
}

/// [R] ORA a,y
fn OP_19(this: *NES) void {
    OPTYPE.AY_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.ORA(this); },
        4 => ALU.ORA(this),
    }
}

/// [R] NOP
fn OP_1A(this: *NES) void {
    OPTYPE.I(this);
}

/// [M] SLO a,y
fn OP_1B(this: *NES) void {
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
fn OP_1C(this: *NES) void {
    OPTYPE.AX_R(this);
}

/// [R] ORA a,x
fn OP_1D(this: *NES) void {
    OPTYPE.AX_M(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.ORA(this); },
        4 => ALU.ORA(this),
    }
}

/// [M] ASL a,x
fn OP_1E(this: *NES) void {
    OPTYPE.AX_M(this);
    switch (this.timing) {
        else => {},
        5 => ALU.ASL(this),
    }
}

/// [M] SLO a,x
fn OP_1F(this: *NES) void {
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
fn OP_20(this: *NES) void {
    this.checkIRQ(5);
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
fn OP_21(this: *NES) void {
    OPTYPE.IX_R(this);
    switch (this.timing) {
        else => {},
        5 => ALU.AND(this),
    }
}

/// STP
fn OP_22(this: *NES) void {
    OPTYPE.STP(this, 0x22);
}

/// [M] RLA (d,x)
fn OP_23(this: *NES) void {
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
fn OP_24(this: *NES) void {
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
fn OP_28(this: *NES) void {
    this.checkIRQ(3);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_getROMWithPC(),
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
fn OP_2C(this: *NES) void {
    OPTYPE.A_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.BIT(this),
    }
}

/// [R] AND a
fn OP_2D(this: *NES) void {
    OPTYPE.A_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.AND(this),
    }
}

/// [M] ROL a
fn OP_2E(this: *NES) void {
    OPTYPE.A_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.ROL(this),
    }
}

/// [M] RLA a
fn OP_2F(this: *NES) void {
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
fn OP_30(this: *NES) void {
    OPTYPE.RA(this, this.p.flags.neg == 1);
}

/// [R] AND (d),y
fn OP_31(this: *NES) void {
    OPTYPE.IY_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.AND(this); },
        5 => ALU.AND(this),
    }
}

/// STP
fn OP_32(this: *NES) void {
    OPTYPE.STP(this, 0x32);
}

/// [M] RLA (d),y
fn OP_33(this: *NES) void {
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
fn OP_34(this: *NES) void {
    OPTYPE.DX_R(this);
}

/// [R] AND d,x
fn OP_35(this: *NES) void {
    OPTYPE.DX_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.AND(this),
    }
}

/// [M] ROL d,x
fn OP_36(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.ROL(this),
    }
}

/// [M] RLA d,x
fn OP_37(this: *NES) void {
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
fn OP_38(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => this.p.flags.carry = 1,
    }
}

/// [R] AND a,y
fn OP_39(this: *NES) void {
    OPTYPE.AY_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.AND(this); },
        4 => ALU.AND(this),
    }
}

/// [R] NOP
fn OP_3A(this: *NES) void {
    OPTYPE.I(this);
}

/// [M] RLA a,y
fn OP_3B(this: *NES) void {
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
fn OP_3C(this: *NES) void {
    OPTYPE.AX_R(this);
}

/// [R] AND a,x
fn OP_3D(this: *NES) void {
    OPTYPE.AX_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.AND(this); },
        4 => ALU.AND(this),
    }
}

/// [M] ROL a,x
fn OP_3E(this: *NES) void {
    OPTYPE.AX_M(this);
    switch (this.timing) {
        else => {},
        5 => ALU.ROL(this),
    }
}

/// [M] RLA a,x
fn OP_3F(this: *NES) void {
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
fn OP_40(this: *NES) void {
    this.checkIRQ(5);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_getROMWithPC(),
        2 => {},
        3 => { const fuckYouZig: u8 = 0x30; this.p.all = (this.p.all & fuckYouZig) | (this.R_pop() & ~fuckYouZig); },
        4 => this.setPC(this.R_pop(), 0),
        5 => this.setPC(this.R_pop(), 1),
    }
}

/// [R] EOR (d,x)
fn OP_41(this: *NES) void {
    OPTYPE.IX_R(this);
    switch (this.timing) {
        else => {},
        5 => ALU.EOR(this),
    }
}

/// STP
fn OP_42(this: *NES) void {
    OPTYPE.STP(this, 0x42);
}

/// [M] SRE (d,x)
fn OP_43(this: *NES) void {
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
fn OP_44(this: *NES) void {
    OPTYPE.D_R(this);
}

/// [R] EOR d
fn OP_45(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.EOR(this),
    }
}

/// [M] LSR d
fn OP_46(this: *NES) void {
    OPTYPE.D_M(this);
    switch (this.timing) {
        else => {},
        3 => ALU.LSR(this),
    }
}

/// [M] SRE d
fn OP_47(this: *NES) void {
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
fn OP_48(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => this.W_push(this.a),
    }
}

/// EOR #i
fn OP_49(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.EOR(this),
    }
}

/// [R] LSR
fn OP_4A(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => ALU.LSRA(this),
    }
}

/// [R] ALR #i
fn OP_4B(this: *NES) void {
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
fn OP_4C(this: *NES) void {
    this.checkIRQ(4);
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
fn OP_4D(this: *NES) void {
    OPTYPE.A_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.EOR(this),
    }
}

/// [M] LSR a
fn OP_4E(this: *NES) void {
    OPTYPE.A_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.LSR(this),
    }
}

/// [M] SRE a
fn OP_4F(this: *NES) void {
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
fn OP_50(this: *NES) void {
    OPTYPE.RA(this, this.p.flags.over == 0);
}

/// [R] EOR (d),y
fn OP_51(this: *NES) void {
    OPTYPE.IY_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.EOR(this); },
        5 => ALU.EOR(this),
    }
}

/// STP
fn OP_52(this: *NES) void {
    OPTYPE.STP(this, 0x52);
}

/// [M] SRE (d),y
fn OP_53(this: *NES) void {
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
fn OP_54(this: *NES) void {
    OPTYPE.DX_R(this);
}

/// [R] EOR d,x
fn OP_55(this: *NES) void {
    OPTYPE.DX_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.EOR(this),
    }
}

/// [M] LSR d,x
fn OP_56(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.LSR(this),
    }
}

/// [M] SRE d,x
fn OP_57(this: *NES) void {
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
fn OP_58(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => this.setI = false,
    }
}

/// [R] EOR a,y
fn OP_59(this: *NES) void {
    OPTYPE.AY_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.EOR(this); },
        4 => ALU.EOR(this),
    }
}

/// [R] NOP
fn OP_5A(this: *NES) void {
    OPTYPE.I(this);
}

/// [M] SRE a,y
fn OP_5B(this: *NES) void {
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
fn OP_5C(this: *NES) void {
    OPTYPE.AX_R(this);
}

/// [R] EOR a,x
fn OP_5D(this: *NES) void {
    OPTYPE.AX_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.EOR(this); },
        4 => ALU.EOR(this),
    }
}

/// [M] LSR a,x
fn OP_5E(this: *NES) void {
    OPTYPE.AX_M(this);
    switch (this.timing) {
        else => {},
        5 => ALU.LSR(this),
    }
}

/// [M] SRE a,x
fn OP_5F(this: *NES) void {
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
fn OP_60(this: *NES) void {
    this.checkIRQ(5);
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
fn OP_61(this: *NES) void {
    OPTYPE.IX_R(this);
    switch (this.timing) {
        else => {},
        5 => ALU.ADC(this),
    }
}

/// STP
fn OP_62(this: *NES) void {
    OPTYPE.STP(this, 0x62);
}

/// [M] RRA (d,x)
fn OP_63(this: *NES) void {
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
fn OP_64(this: *NES) void {
    OPTYPE.D_R(this);
}

/// [R] ADC d
fn OP_65(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.ADC(this),
    }
}

/// [M] ROR d
fn OP_66(this: *NES) void {
    OPTYPE.D_M(this);
    switch (this.timing) {
        else => {},
        3 => ALU.ROR(this),
    }
}

/// [M] RRA d
fn OP_67(this: *NES) void {
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
fn OP_68(this: *NES) void {
    this.checkIRQ(3);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.R_getROMWithPC(),
        2 => {},
        3 => {
            this.a = this.R_pop();

            this.p.flags.zero = @intFromBool(this.a == 0x00);
            this.p.flags.neg  = @intFromBool(this.a >= 0x80);
        },
    }
}

/// ADC #i
fn OP_69(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.ADC(this),
    }
}

/// ROR
fn OP_6A(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => ALU.RORA(this),
    }
}

/// ARR #i
fn OP_6B(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => {
            ALU.AND(this);
            ALU.RORA(this);

            this.p.flags.carry = @intFromBool((this.a & 0x40) == 0x40);
            this.p.flags.over  = @intFromBool((this.a & 0x40) == 0x40) ^ @intFromBool((this.a & 0x20) == 0x20);
        }
    }
}

/// JMP (a)
fn OP_6C(this: *NES) void {
    this.checkIRQ(4);
    
    switch (this.timing) {
        else => this.resetTiming(),
        1 => { this.R_readROM(); this.setAB(this.data, 0); },
        2 => { this.R_readROM(); this.setAB(this.data, 1); },
        3 => { this.R_getROMWithAB(); this.setPC(this.data, 0); this.ABAdd(1, 0); },
        4 => { this.R_getROMWithAB(); this.setPC(this.data, 1); },
    }
}

/// [R] ADC a
fn OP_6D(this: *NES) void {
    OPTYPE.A_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.ADC(this),
    }
}

/// [M] ROR a
fn OP_6E(this: *NES) void {
    OPTYPE.A_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.ROR(this),
    }
}

/// [M] ROR a
fn OP_6F(this: *NES) void {
    OPTYPE.A_M(this);
    switch (this.timing) {
        else => {},
        4 => {
            ALU.ROR(this);
            ALU.ADC(this);
        },
    }
}

/// BVS
fn OP_70(this: *NES) void {
    OPTYPE.RA(this, this.p.flags.over == 1);
}

/// [R] ADC (d),y
fn OP_71(this: *NES) void {
    OPTYPE.IY_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.ADC(this); },
        5 => ALU.ADC(this),
    }
}

/// STP
fn OP_72(this: *NES) void {
    OPTYPE.STP(this, 0x72);
}

/// [M] RRA (d),y
fn OP_73(this: *NES) void {
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
fn OP_74(this: *NES) void {
    OPTYPE.DX_R(this);
}

/// [R] ADC d,x
fn OP_75(this: *NES) void {
    OPTYPE.DX_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.ADC(this),
    }
}

/// [M] ROR d,x
fn OP_76(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.ROR(this),
    }
}

/// [M] RRA d,x
fn OP_77(this: *NES) void {
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
fn OP_78(this: *NES) void {
    this.checkIRQ(1);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.setI = true,
    }
}

/// [R] ADC a,y
fn OP_79(this: *NES) void {
    OPTYPE.AY_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.ADC(this); },
        4 => ALU.ADC(this),
    }
}

/// NOP
fn OP_7A(this: *NES) void {
    OPTYPE.I(this);
}

/// [M] RRA a,y
fn OP_7B(this: *NES) void {
    OPTYPE.AY_M(this);
    switch (this.timing) {
        else => {},
        5 => {
            ALU.ROR(this);
            ALU.ADC(this);
        },
    }
}

/// [R] NOP a,x
fn OP_7C(this: *NES) void {
    OPTYPE.AX_R(this);
}

/// [R] ADC a,x
fn OP_7D(this: *NES) void {
    OPTYPE.AX_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.ADC(this); },
        4 => ALU.ADC(this),
    }
}

/// [M] ROR a,x
fn OP_7E(this: *NES) void {
    OPTYPE.AX_M(this);
    switch (this.timing) {
        else => {},
        5 => ALU.ROR(this),
    }
}

/// [M] RRA a,x
fn OP_7F(this: *NES) void {
    OPTYPE.AX_M(this);
    switch (this.timing) {
        else => {},
        5 => {
            ALU.ROR(this);
            ALU.ADC(this);
        },
    }
}

/// [R] NOP #i
fn OP_80(this: *NES) void {
    OPTYPE.M(this);
}

/// [W] STA (d,x)
fn OP_81(this: *NES) void {
    OPTYPE.IX_W(this);
    switch (this.timing) {
        else => {},
        5 => this.W_writeROMV(this.a),
    }
}

/// [R] NOP #i
fn OP_82(this: *NES) void {
    OPTYPE.M(this);
}

/// [W] STA (d,x)
fn OP_83(this: *NES) void {
    OPTYPE.IX_W(this);
    switch (this.timing) {
        else => {},
        5 => this.W_writeROMV(this.a & this.x),
    }
}

/// [W] STY d
fn OP_84(this: *NES) void {
    OPTYPE.D_W(this);
    switch (this.timing) {
        else => {},
        2 => this.W_writeROMVD(this.y),
    }
}

/// [W] STA d
fn OP_85(this: *NES) void {
    OPTYPE.D_W(this);
    switch (this.timing) {
        else => {},
        2 => this.W_writeROMVD(this.a),
    }
}

/// [W] STX d
fn OP_86(this: *NES) void {
    OPTYPE.D_W(this);
    switch (this.timing) {
        else => {},
        2 => this.W_writeROMVD(this.x),
    }
}

/// [W] SAX d
fn OP_87(this: *NES) void {
    OPTYPE.D_W(this);
    switch (this.timing) {
        else => {},
        2 => this.W_writeROMVD(this.a & this.x),
    }
}

/// DEY
fn OP_88(this: *NES) void {
    this.checkIRQ(1);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => {
            this.y, _ = @subWithOverflow(this.y, 1);

            this.p.flags.zero = @intFromBool(this.y == 0x00);
            this.p.flags.neg  = @intFromBool(this.y >= 0x80);
        },
    }
}

/// NOP #i
fn OP_89(this: *NES) void {
    OPTYPE.M(this);
}

/// TXA
fn OP_8A(this: *NES) void {
    this.checkIRQ(1);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => ALU.TXA(this),
    }
}

/// [R] XAA #i
fn OP_8B(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => this.a = (this.a | 0xEE) & this.x & this.data,
    }
}

/// [W] STY a
fn OP_8C(this: *NES) void {
    OPTYPE.A_W(this);
    switch (this.timing) {
        else => {},
        3 => this.W_writeROMV(this.y),
    }
}

/// [W] STA a
fn OP_8D(this: *NES) void {
    OPTYPE.A_W(this);
    switch (this.timing) {
        else => {},
        3 => this.W_writeROMV(this.a),
    }
}

/// [W] STX a
fn OP_8E(this: *NES) void {
    OPTYPE.A_W(this);
    switch (this.timing) {
        else => {},
        3 => this.W_writeROMV(this.x),
    }
}

/// [W] SAX a
fn OP_8F(this: *NES) void {
    OPTYPE.A_W(this);
    switch (this.timing) {
        else => {},
        3 => this.W_writeROMV(this.a & this.x),
    }
}

/// BCC
fn OP_90(this: *NES) void {
    OPTYPE.RA(this, this.p.flags.carry == 0);
}

/// [W] STA (d),y
fn OP_91(this: *NES) void {
    OPTYPE.IY_W(this);
    switch (this.timing) {
        else => {},
        5 => this.W_writeROMV(this.a),
    }
}

/// STP
fn OP_92(this: *NES) void {
    OPTYPE.STP(this, 0x92);
}

/// [M] SHA (d),y
fn OP_93(this: *NES) void {
    OPTYPE.IY_M(this);
    switch (this.timing) {
        else => {},
        6 => {
            var pc: u8 = @truncate(this.pc >> 8);
            pc, _ = @addWithOverflow(pc, 1);
            this.W_writeROMVD(this.a & this.x & pc);
        },
    }
}

/// [W] STY d,x
fn OP_94(this: *NES) void {
    OPTYPE.DX_W(this);
    switch (this.timing) {
        else => {},
        3 => this.W_writeROMVD(this.y),
    }
}

/// [W] STA d,x
fn OP_95(this: *NES) void {
    OPTYPE.DX_W(this);
    switch (this.timing) {
        else => {},
        3 => this.W_writeROMVD(this.a),
    }
}

/// [W] STX d,y
fn OP_96(this: *NES) void {
    OPTYPE.DY_W(this);
    switch (this.timing) {
        else => {},
        3 => this.W_writeROMVD(this.x),
    }
}

/// [W] SAX d,y
fn OP_97(this: *NES) void {
    OPTYPE.DY_W(this);
    switch (this.timing) {
        else => {},
        3 => this.W_writeROMVD(this.a & this.x),
    }
}

/// TYA
fn OP_98(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => ALU.TYA(this),
    }
}

/// [W] STA a,y
fn OP_99(this: *NES) void {
    OPTYPE.AY_W(this);
    switch (this.timing) {
        else => {},
        4 => this.W_writeROMV(this.a),
    }
}

/// TXS
fn OP_9A(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => ALU.TXS(this),
    }
}

/// [W] TAS a,y
fn OP_9B(this: *NES) void {
    OPTYPE.AY_W(this);
    switch (this.timing) {
        else => {},
        4 => {
            this.sp = this.x & this.a;
            
            var pc: u8 = @truncate(this.pc >> 8);
            pc, _ = @addWithOverflow(pc, 1);
            this.W_writeROMV(this.a & this.x & pc);

            this.p.flags.zero  = @intFromBool(this.sp == 0x00);
            this.p.flags.neg   = @intFromBool(this.sp >= 0x80);
        },
    }
}

// TODO: implement that weird caveat to these SHx commands

/// [W] SHY a,x
fn OP_9C(this: *NES) void {
    OPTYPE.AX_W(this);
    switch (this.timing) {
        else => {},
        4 => {
            var pc: u8 = @truncate(this.pc >> 8);
            pc, _ = @addWithOverflow(pc, 1);
            this.W_writeROMV(this.y & pc);
        },
    }
}

/// [W] STA a,x
fn OP_9D(this: *NES) void {
    OPTYPE.AX_W(this);
    switch (this.timing) {
        else => {},
        4 => this.W_writeROMV(this.a),
    }
}

/// [W] SHX a,y
fn OP_9E(this: *NES) void {
    OPTYPE.AX_W(this);
    switch (this.timing) {
        else => {},
        4 => {
            var pc: u8 = @truncate(this.pc >> 8);
            pc, _ = @addWithOverflow(pc, 1);
            this.W_writeROMV(this.x & pc);
        },
    }
}

/// [W] SHA a,y
fn OP_9F(this: *NES) void {
    OPTYPE.AX_W(this);
    switch (this.timing) {
        else => {},
        4 => {
            var pc: u8 = @truncate(this.pc >> 8);
            pc, _ = @addWithOverflow(pc, 1);
            this.W_writeROMV(this.x & this.a & pc);
        },
    }
}

// ...

/// [R] LDY #i
fn OP_A0(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.LDY(this),
    }
}

/// [R] LDA (d,x)
fn OP_A1(this: *NES) void {
    OPTYPE.IX_R(this);
    switch (this.timing) {
        else => {},
        5 => ALU.LDA(this),
    }
}

/// [R] LDX #i
fn OP_A2(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.LDX(this),
    }
}

/// [R] LAX (d,x)
fn OP_A3(this: *NES) void {
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
fn OP_A4(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.LDY(this),
    }
}

/// [R] LDA d
fn OP_A5(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.LDA(this),
    }
}

/// [R] LDX d
fn OP_A6(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.LDX(this),
    }
}

/// [R] LAX d
fn OP_A7(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => {
            ALU.LDA(this);
            ALU.TAX(this);
        },
    }
}

/// TAY
fn OP_A8(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => ALU.TAY(this),
    }
}

/// [R] LDA #i
fn OP_A9(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.LDA(this),
    }
}

/// TAX
fn OP_AA(this: *NES) void {
    this.checkIRQ(2);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => ALU.TAX(this),
    }
}

/// [R] LAX #i
fn OP_AB(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => {
            ALU.LDA(this);
            ALU.TAX(this);
        },
    }
}

/// [R] LDY a
fn OP_AC(this: *NES) void {
    OPTYPE.A_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.LDY(this),
    }
}

/// [R] LDA a
fn OP_AD(this: *NES) void {
    OPTYPE.A_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.LDA(this),
    }
}

/// [R] LDX a
fn OP_AE(this: *NES) void {
    OPTYPE.A_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.LDX(this),
    }
}

/// [R] LAX a
fn OP_AF(this: *NES) void {
    OPTYPE.A_R(this);
    switch (this.timing) {
        else => {},
        3 => {
            ALU.LDA(this);
            ALU.TAX(this);
        },
    }
}

/// BCS
fn OP_B0(this: *NES) void {
    OPTYPE.RA(this, this.p.flags.carry == 1);
}

/// [R] LDA (d),y
fn OP_B1(this: *NES) void {
    OPTYPE.IY_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.LDA(this); },
        5 => ALU.LDA(this),
    }
}

/// STP
fn OP_B2(this: *NES) void {
    OPTYPE.STP(this, 0xB2);
}

/// [M] LAX (d),y
fn OP_B3(this: *NES) void {
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
fn OP_B4(this: *NES) void {
    OPTYPE.DX_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.LDY(this),
    }
}

/// [R] LDA d,x
fn OP_B5(this: *NES) void {
    OPTYPE.DX_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.LDA(this),
    }
}

/// [R] LDX d,y
fn OP_B6(this: *NES) void {
    OPTYPE.DY_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.LDX(this),
    }
}

/// [R] LAX d,y
fn OP_B7(this: *NES) void {
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
fn OP_B8(this: *NES) void {
    this.checkIRQ(1);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.p.flags.over = 0,
    }
}

/// [R] LDA a,y
fn OP_B9(this: *NES) void {
    OPTYPE.AY_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.LDA(this); },
        4 => ALU.LDA(this),
    }
}

/// TSX
fn OP_BA(this: *NES) void {
    OPTYPE.I(this);
    switch (this.timing) {
        else => {},
        1 => ALU.TSX(this),
    }
}

/// [M] LAS a,y
fn OP_BB(this: *NES) void {
    OPTYPE.AY_M(this);
    switch (this.timing) {
        else => {},
        5 => {
            const val = this.data & this.sp;
            this.a  = val;
            this.x  = val;
            this.sp = val;
        },
    }
}

/// [R] LDY a,x
fn OP_BC(this: *NES) void {
    OPTYPE.AX_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.LDY(this); },
        4 => ALU.LDY(this),
    }
}

/// [R] LDA a,x
fn OP_BD(this: *NES) void {
    OPTYPE.AX_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.LDA(this); },
        4 => ALU.LDA(this),
    }
}

/// [R] LDX a,y
fn OP_BE(this: *NES) void {
    OPTYPE.AY_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.LDX(this); },
        4 => ALU.LDX(this),
    }
}

/// [R] LAX a,y
fn OP_BF(this: *NES) void {
    OPTYPE.AX_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) {
            ALU.LDA(this);
            ALU.TAX(this);
        },
        4 => {
            ALU.LDA(this);
            ALU.TAX(this);
        },
    }
}

/// CPY #i
fn OP_C0(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.CPY(this),
    }
}

/// [R] CMP (d,x)
fn OP_C1(this: *NES) void {
    OPTYPE.IX_R(this);
    switch (this.timing) {
        else => {},
        5 => ALU.CMP(this),
    }
}

/// [R] NOP #i
fn OP_C2(this: *NES) void {
    OPTYPE.M(this);
}

/// [R] DCP (d,x)
fn OP_C3(this: *NES) void {
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
fn OP_C4(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.CPY(this),
    }
}

/// [R] CMP d
fn OP_C5(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.CMP(this),
    }
}

/// [M] DEC d
fn OP_C6(this: *NES) void {
    OPTYPE.D_M(this);
    switch (this.timing) {
        else => {},
        3 => ALU.DEC(this),
    }
}

/// [M] DCP d
fn OP_C7(this: *NES) void {
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
fn OP_C8(this: *NES) void {
    this.checkIRQ(1);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => {
            this.y, _ = @addWithOverflow(this.y, 1);

            this.p.flags.zero = @intFromBool(this.y == 0x00);
            this.p.flags.neg  = @intFromBool(this.y >= 0x80);
        },
    }
}

/// [R] CMP #i
fn OP_C9(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.CMP(this),
    }
}

/// DEX
fn OP_CA(this: *NES) void {
    this.checkIRQ(1);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => {
            this.x, _ = @subWithOverflow(this.x, 1);

            this.p.flags.zero = @intFromBool(this.x == 0x00);
            this.p.flags.neg  = @intFromBool(this.x >= 0x80);
        },
    }
}

/// AXS #i
fn OP_CB(this: *NES) void {
    this.checkIRQ(1);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => {
            this.x, const c: u1 = @subWithOverflow(this.a & this.x, this.data);

            this.p.flags.carry = ~c;
            this.p.flags.zero = @intFromBool(this.x == 0x00);
            this.p.flags.neg  = @intFromBool(this.x >= 0x80);
        },
    }
}

/// [R] CPY a
fn OP_CC(this: *NES) void {
    OPTYPE.A_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.CPY(this),
    }
}

/// [R] CMP a
fn OP_CD(this: *NES) void {
    OPTYPE.A_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.CMP(this),
    }
}

/// [M] ASL a
fn OP_CE(this: *NES) void {
    OPTYPE.A_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.DEC(this),
    }
}

/// [M] ASL a
fn OP_CF(this: *NES) void {
    OPTYPE.A_M(this);
    switch (this.timing) {
        else => {},
        4 => {
            ALU.DEC(this);
            ALU.CMP(this);
        },
    }
}

/// BNE
fn OP_D0(this: *NES) void {
    OPTYPE.RA(this, this.p.flags.zero == 0);
}

/// [R] CMP (d),y
fn OP_D1(this: *NES) void {
    OPTYPE.IY_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.CMP(this); },
        5 => ALU.CMP(this),
    }
}

/// STP
fn OP_D2(this: *NES) void {
    OPTYPE.STP(this, 0xD2);
}

/// [M] DCP (d),y
fn OP_D3(this: *NES) void {
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
fn OP_D4(this: *NES) void {
    OPTYPE.DX_R(this);
}

/// [R] CMP d,x
fn OP_D5(this: *NES) void {
    OPTYPE.DX_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.CMP(this),
    }
}

/// [M] DEC d,x
fn OP_D6(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.DEC(this),
    }
}

/// [M] DCP d,x
fn OP_D7(this: *NES) void {
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
fn OP_D8(this: *NES) void {
    this.checkIRQ(1);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => this.p.flags.dec = 0,
    }
}



/// CPX #i
fn OP_E0(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.CPX(this),
    }
}

/// [R] SBC (d,x)
fn OP_E1(this: *NES) void {
    OPTYPE.IX_R(this);
    switch (this.timing) {
        else => {},
        5 => ALU.SBC(this),
    }
}

/// NOP #i
fn OP_E2(this: *NES) void {
    OPTYPE.M(this);
}

/// [M] ISC (d,x)
fn OP_E3(this: *NES) void {
    OPTYPE.IX_M(this);
    switch (this.timing) {
        else => {},
        6 => {
            ALU.INC(this);
            ALU.SBC(this);
        },
    }
}

/// [R] CPX d
fn OP_E4(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.CPX(this),
    }
}

/// [R] SBC d
fn OP_E5(this: *NES) void {
    OPTYPE.D_R(this);
    switch (this.timing) {
        else => {},
        2 => ALU.SBC(this),
    }
}

/// [M] INC d
fn OP_E6(this: *NES) void {
    OPTYPE.D_M(this);
    switch (this.timing) {
        else => {},
        3 => ALU.INC(this),
    }
}

/// [M] ISC d
fn OP_E7(this: *NES) void {
    OPTYPE.D_M(this);
    switch (this.timing) {
        else => {},
        3 => {
            ALU.INC(this);
            ALU.SBC(this);
        },
    }
}

/// INX
fn OP_E8(this: *NES) void {
    this.checkIRQ(1);
    switch (this.timing) {
        else => this.resetTiming(),
        1 => {
            this.x, _ = @addWithOverflow(this.x, 1);

            this.p.flags.zero = @intFromBool(this.x == 0x00);
            this.p.flags.neg  = @intFromBool(this.x >= 0x80);
        },
    }
}

/// SBC #i
fn OP_E9(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.SBC(this),
    }
}

/// NOP
fn OP_EA(this: *NES) void {
    OPTYPE.I(this);
}

/// SBC #i
fn OP_EB(this: *NES) void {
    OPTYPE.M(this);
    switch (this.timing) {
        else => {},
        1 => ALU.SBC(this),
    }
}

/// [R] CPX a
fn OP_EC(this: *NES) void {
    OPTYPE.A_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.CPX(this),
    }
}

/// [R] SBC a
fn OP_ED(this: *NES) void {
    OPTYPE.A_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.SBC(this),
    }
}

/// [M] INC a
fn OP_EE(this: *NES) void {
    OPTYPE.A_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.INC(this),
    }
}

/// [M] ISC a
fn OP_EF(this: *NES) void {
    OPTYPE.A_M(this);
    switch (this.timing) {
        else => {},
        4 => {
            ALU.INC(this);
            ALU.SBC(this);
        },
    }
}

/// BEQ
fn OP_F0(this: *NES) void {
    OPTYPE.RA(this, this.p.flags.zero == 1);
}

/// [R] SBC (d),y
fn OP_F1(this: *NES) void {
    OPTYPE.IY_R(this);
    switch (this.timing) {
        else => {},
        0xE => if (this.add.flags.carry == 0) { ALU.SBC(this); },
        5 => ALU.SBC(this),
    }
}

/// STP
fn OP_F2(this: *NES) void {
    OPTYPE.STP(this, 0xF2);
}

/// [M] ISC (d),y
fn OP_F3(this: *NES) void {
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
fn OP_F4(this: *NES) void {
    OPTYPE.DX_R(this);
}

/// [R] SBC d,x
fn OP_F5(this: *NES) void {
    OPTYPE.DX_R(this);
    switch (this.timing) {
        else => {},
        3 => ALU.SBC(this),
    }
}

/// [M] INC d,x
fn OP_F6(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => ALU.INC(this),
    }
}

/// [M] ISC d,x
fn OP_F7(this: *NES) void {
    OPTYPE.DX_M(this);
    switch (this.timing) {
        else => {},
        4 => {
            ALU.INC(this);
            ALU.SBC(this);
        },
    }
}

fn OP_F8(this: *NES) void {
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
    OP_60, OP_61, OP_62, OP_63, OP_64, OP_65, OP_66, OP_67, OP_68, OP_69, OP_6A, OP_6B, OP_6C, OP_6D, OP_6E, OP_6F,
    OP_70, OP_71, OP_72, OP_73, OP_74, OP_75, OP_76, OP_77, OP_78, OP_79, OP_7A, OP_7B, OP_7C, OP_7D, OP_7E, OP_7F,
    OP_80, OP_81, OP_82, OP_83, OP_84, OP_85, OP_86, OP_87, OP_88, OP_89, OP_8A, OP_8B, OP_8C, OP_8D, OP_8E, OP_8F,
    OP_90, OP_91, OP_92, OP_93, OP_94, OP_95, OP_96, OP_97, OP_98, OP_99, OP_9A, OP_9B, OP_9C, OP_9D, OP_9E, OP_9F,
    OP_A0, OP_A1, OP_A2, OP_A3, OP_A4, OP_A5, OP_A6, OP_A7, OP_A8, OP_A9, OP_AA, OP_AB, OP_AC, OP_AD, OP_AE, OP_AF,
    OP_B0, OP_B1, OP_B2, OP_B3, OP_B4, OP_B5, OP_B6, OP_B7, OP_B8, OP_B9, OP_BA, OP_BB, OP_BC, OP_BD, OP_BE, OP_BF,
    OP_C0, OP_C1, OP_C2, OP_C3, OP_C4, OP_C5, OP_C6, OP_C7, OP_C8, OP_C9, OP_CA, OP_CB, OP_CC, OP_CD, OP_CE, OP_CF,
    OP_D0, OP_D1, OP_D2, OP_D3, OP_D4, OP_D5, OP_D6, OP_D7, OP_D8, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02,
    OP_E0, OP_E1, OP_E2, OP_E3, OP_E4, OP_E5, OP_E6, OP_E7, OP_E8, OP_E9, OP_EA, OP_EB, OP_EC, OP_ED, OP_EE, OP_EF,
    OP_F0, OP_F1, OP_F2, OP_F3, OP_F4, OP_F5, OP_F6, OP_F7, OP_F8, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02, OP_02,
};