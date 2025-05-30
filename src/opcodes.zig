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
        2 => this.W_push(this.p.all),
    }
}

/// [R] ORA #i
pub fn OP_09(this: *NES) void {
    OPTYPE.I(this);
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
    OPTYPE.I(this);
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
        2 => {},
        3 => this.W_push(@truncate(this.pc >> 0)),
        4 => this.W_push(@truncate(this.pc >> 8)),
        5 => {
            this.setPC(this.data, 0);
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
        3 => this.p.all = this.W_pop(),
    }
}

/// [R] AND #i
fn OP_29(this: *NES) void {
    OPTYPE.I(this);
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
    OPTYPE.I(this);
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
        4 => if (this.add.flags.carry == 0) { ALU.ORA(this); },
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
        3 => if (this.add.flags.carry == 0) { ALU.ORA(this); },
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
        3 => this.p.all = this.W_pop(),
        4 => this.setPC(this.W_pop(), 1),
        5 => this.setPC(this.W_pop(), 0),
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
        3 => { this.R_getROMWithAB(); this.data = this.ab.half.high; },
        4 => {
            this.setPC(this.data, 0);
            this.data = this.ab.half.low;
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
        1 => this.setI = true,
    }
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

pub const opTable = [_]*const fn(*NES) void{
    OP_00, OP_01, OP_02, OP_03, OP_04, OP_05, OP_06, OP_07, OP_08, OP_09, OP_0A, OP_0B, OP_0C, OP_0D, OP_0E, OP_0F,
    OP_10, OP_11, OP_12, OP_13, OP_14, OP_15, OP_16, OP_17, OP_18, OP_19, OP_1A, OP_1B, OP_1C, OP_1D, OP_1E, OP_1F,
    OP_20, OP_21, OP_22, OP_23, OP_24, OP_25, OP_26, OP_27, OP_28, OP_29, OP_2A, OP_2B, OP_2C, OP_2D, OP_2E, OP_2F,
    OP_30, OP_31, OP_32, OP_33, OP_34, OP_35, OP_36, OP_37, OP_38, OP_39, OP_3A, OP_3B, OP_3C, OP_3D, OP_3E, OP_3F,
    OP_40, OP_41, OP_42, OP_43, OP_44, OP_45, OP_46, OP_47, OP_48, OP_49, OP_4A, OP_4B, OP_4C, OP_4D, OP_4E, OP_4F,
    OP_50, OP_51, OP_52, OP_53, OP_54, OP_55, OP_56, OP_57, OP_58, //OP_59, OP_5A, OP_5B, OP_5C, OP_5D, OP_5E, OP_5F,
    //OP_60, OP_61, OP_62, OP_63, OP_64, OP_65, OP_66, OP_67, OP_68, OP_69, OP_6A, OP_6B, OP_6C, OP_6D, OP_6E, OP_6F,
    //OP_70, OP_71, OP_72, OP_73, OP_74, OP_75, OP_76, OP_77, OP_78, OP_79, OP_7A, OP_7B, OP_7C, OP_7D, OP_7E, OP_7F,
    //OP_80, OP_81, OP_82, OP_83, OP_84, OP_85, OP_86, OP_87, OP_88, OP_89, OP_8A, OP_8B, OP_8C, OP_8D, OP_8E, OP_8F,
    //OP_90, OP_91, OP_92, OP_93, OP_94, OP_95, OP_96, OP_97, OP_98, OP_99, OP_9A, OP_9B, OP_9C, OP_9D, OP_9E, OP_9F,
    OP_A0, OP_A1, OP_A2, //OP_A3, OP_A4, OP_A5, OP_A6, OP_A7, OP_A8, OP_A9, OP_AA, OP_AB, OP_AC, OP_AD, OP_AE, OP_AF,
    //OP_B0, OP_B1, OP_B2, OP_B3, OP_B4, OP_B5, OP_B6, OP_B7, OP_B8, OP_B9, OP_BA, OP_BB, OP_BC, OP_BD, OP_BE, OP_BF,
    //OP_C0, OP_C1, OP_C2, OP_C3, OP_C4, OP_C5, OP_C6, OP_C7, OP_C8, OP_C9, OP_CA, OP_CB, OP_CC, OP_CD, OP_CE, OP_CF,
    //OP_D0, OP_D1, OP_D2, OP_D3, OP_D4, OP_D5, OP_D6, OP_D7, OP_D8, OP_D9, OP_DA, OP_DB, OP_DC, OP_DD, OP_DE, OP_DF,
    //OP_E0, OP_E1, OP_E2, OP_E3, OP_E4, OP_E5, OP_E6, OP_E7, OP_E8, OP_E9, OP_EA, OP_EB, OP_EC, OP_ED, OP_EE, OP_EF,
    //OP_F0, OP_F1, OP_F2, OP_F3, OP_F4, OP_F5, OP_F6, OP_F7, OP_F8, OP_F9, OP_FA, OP_FB, OP_FC, OP_FD, OP_FE, OP_FF,
};