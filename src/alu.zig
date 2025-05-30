const NES = @import("nes.zig").NES;

pub fn AND(this: *NES) void {
    this.a &= this.data;

    this.p.flags.zero = @intFromBool(this.a == 0x00);
    this.p.flags.neg  = @intFromBool(this.a >= 0x80);
}

pub fn ASL(this: *NES) void {
    const prevC: u1 = this.p.flags.carry;

    this.data, this.p.flags.carry = @shlWithOverflow(this.data, 1);
    this.data += @intCast(prevC);

    this.p.flags.zero = @intFromBool(this.data == 0x00);
    this.p.flags.neg  = @intFromBool(this.data >= 0x80);
}

pub fn ASLA(this: *NES) void {
    const prevC: u1 = this.p.flags.carry;

    this.a, this.p.flags.carry = @shlWithOverflow(this.a, 1);
    this.a += @intCast(prevC);

    this.p.flags.zero = @intFromBool(this.a == 0x00);
    this.p.flags.neg  = @intFromBool(this.a >= 0x80);
}

pub fn EOR(this: *NES) void {
    this.a ^= this.data;

    this.p.flags.zero = @intFromBool(this.a == 0x00);
    this.p.flags.neg  = @intFromBool(this.a >= 0x80);
}

pub fn BIT(this: *NES) void {
    const bit: u8 = this.a & this.data;

    this.p.flags.zero = @intFromBool(bit == 0x00);
    this.p.flags.over = @intFromBool((bit & 0x40) == 0x40);
    this.p.flags.neg  = @intFromBool(bit >= 0x80);
}

pub fn LDA(this: *NES) void {
    this.a = this.data;

    this.p.flags.zero = @intFromBool(this.a == 0x00);
    this.p.flags.neg  = @intFromBool(this.a >= 0x80);
}

pub fn LSR(this: *NES) void {
    const c: u1 = @intCast(this.a & 1);
    this.data >>= 1;

    this.p.flags.carry = c;
    this.p.flags.zero  = @intFromBool(this.data == 0x00);
    this.p.flags.neg   = @intFromBool(this.data >= 0x80);
}

pub fn LSRA(this: *NES) void {
    const c: u1 = @intCast(this.a & 1);
    this.a >>= 1;

    this.p.flags.carry = c;
    this.p.flags.zero  = @intFromBool(this.a == 0x00);
    this.p.flags.neg   = @intFromBool(this.a >= 0x80);
}

pub fn ORA(this: *NES) void {
    this.a |= this.data;

    this.p.flags.zero = @intFromBool(this.a == 0x00);
    this.p.flags.neg  = @intFromBool(this.a >= 0x80);
}

pub fn ROL(this: *NES) void {
    const i: i8 = @intCast(this.data);
    const c: bool = i < 0;

    this.data <<= 1;
    this.data += this.p.flags.carry;

    this.p.flags.carry = @intFromBool(c);
    this.p.flags.zero  = @intFromBool(this.data == 0x00);
    this.p.flags.neg   = @intFromBool(this.data >= 0x80);
}

pub fn ROLA(this: *NES) void {
    const i: i8 = @intCast(this.data);
    const c: bool = i < 0;

    this.a <<= 1;
    this.a += this.p.flags.carry;

    this.p.flags.carry = @intFromBool(c);
    this.p.flags.zero  = @intFromBool(this.a == 0x00);
    this.p.flags.neg   = @intFromBool(this.a >= 0x80);
}