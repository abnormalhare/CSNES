const NES = @import("nes.zig").NES;

pub fn ADC(this: *NES) void {
    const a: u8 = this.a;
    this.a, const c1: u1 = @addWithOverflow(this.a, this.data);
    this.a, const c2: u1 = @addWithOverflow(this.a, this.p.flags.carry);

    this.p.flags.carry = c1 | c2;
    this.p.flags.zero = @intFromBool(this.a == 0x00);
    this.p.flags.over = @intFromBool(((this.a ^ a) & (this.a ^ this.data) & 0x80) == 0x80);
    this.p.flags.neg  = @intFromBool(this.a >= 0x80);
}

pub fn AND(this: *NES) void {
    this.a &= this.data;

    this.p.flags.zero = @intFromBool(this.a == 0x00);
    this.p.flags.neg  = @intFromBool(this.a >= 0x80);
}

pub fn ASL(this: *NES) void {
    this.data, this.p.flags.carry = @shlWithOverflow(this.data, 1);

    this.p.flags.zero = @intFromBool(this.data == 0x00);
    this.p.flags.neg  = @intFromBool(this.data >= 0x80);
}

pub fn ASLA(this: *NES) void {
    this.a, this.p.flags.carry = @shlWithOverflow(this.a, 1);

    this.p.flags.zero = @intFromBool(this.a == 0x00);
    this.p.flags.neg  = @intFromBool(this.a >= 0x80);
}

pub fn BIT(this: *NES) void {
    const bit: u8 = this.a & this.data;

    this.p.flags.zero = @intFromBool(bit == 0x00);
    this.p.flags.over = @intFromBool((this.data & 0x40) == 0x40);
    this.p.flags.neg  = @intFromBool(this.data >= 0x80);
}

pub fn CMP(this: *NES) void {
    const res: u8, _ = @subWithOverflow(this.a, this.data);

    this.p.flags.carry = @intFromBool(this.a >= this.data);
    this.p.flags.zero  = @intFromBool(this.a == this.data);
    this.p.flags.neg   = @intFromBool(res >= 0x80);
}

pub fn CPX(this: *NES) void {
    const res: u8, _ = @subWithOverflow(this.x, this.data);

    this.p.flags.carry = @intFromBool(this.x >= this.data);
    this.p.flags.zero  = @intFromBool(this.x == this.data);
    this.p.flags.neg   = @intFromBool(res >= 0x80);
}

pub fn CPY(this: *NES) void {
    const res: u8, _ = @subWithOverflow(this.y, this.data);

    this.p.flags.carry = @intFromBool(this.y >= this.data);
    this.p.flags.zero  = @intFromBool(this.y == this.data);
    this.p.flags.neg   = @intFromBool(res >= 0x80);
}

pub fn DEC(this: *NES) void {
    this.data, _ = @subWithOverflow(this.data, 1);

    this.p.flags.zero = @intFromBool(this.data == 0x00);
    this.p.flags.neg  = @intFromBool(this.data >= 0x80);
}

pub fn EOR(this: *NES) void {
    this.a ^= this.data;

    this.p.flags.zero = @intFromBool(this.a == 0x00);
    this.p.flags.neg  = @intFromBool(this.a >= 0x80);
}

pub fn INC(this: *NES) void {
    this.data, _ = @addWithOverflow(this.data, 1);

    this.p.flags.zero = @intFromBool(this.data == 0x00);
    this.p.flags.neg  = @intFromBool(this.data >= 0x80);
}

pub fn LDA(this: *NES) void {
    this.a = this.data;

    this.p.flags.zero = @intFromBool(this.a == 0x00);
    this.p.flags.neg  = @intFromBool(this.a >= 0x80);
}

pub fn LDX(this: *NES) void {
    this.x = this.data;

    this.p.flags.zero = @intFromBool(this.x == 0x00);
    this.p.flags.neg  = @intFromBool(this.x >= 0x80);
}

pub fn LDY(this: *NES) void {
    this.y = this.data;

    this.p.flags.zero = @intFromBool(this.y == 0x00);
    this.p.flags.neg  = @intFromBool(this.y >= 0x80);
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
    const c: bool = this.data >= 0x80;

    this.data <<= 1;
    this.data += this.p.flags.carry;

    this.p.flags.carry = @intFromBool(c);
    this.p.flags.zero  = @intFromBool(this.data == 0x00);
    this.p.flags.neg   = @intFromBool(this.data >= 0x80);
}

pub fn ROLA(this: *NES) void {
    const c: bool = this.a >= 0x80;

    this.a <<= 1;
    this.a += this.p.flags.carry;

    this.p.flags.carry = @intFromBool(c);
    this.p.flags.zero  = @intFromBool(this.a == 0x00);
    this.p.flags.neg   = @intFromBool(this.a >= 0x80);
}

pub fn ROR(this: *NES) void {
    const c: bool = (this.data & 1) == 1;

    this.data >>= 1;
    this.data = this.data + @as(u8, this.p.flags.carry) * 0x80;

    this.p.flags.carry = @intFromBool(c);
    this.p.flags.zero  = @intFromBool(this.data == 0x00);
    this.p.flags.neg   = @intFromBool(this.data >= 0x80);
}

pub fn RORA(this: *NES) void {
    const c: bool = (this.a & 1) == 1;

    this.a >>= 1;
    this.a = this.a + @as(u8, this.p.flags.carry) * 0x80;

    this.p.flags.carry = @intFromBool(c);
    this.p.flags.zero  = @intFromBool(this.a == 0x00);
    this.p.flags.neg   = @intFromBool(this.a >= 0x80);
}

pub fn SBC(this: *NES) void {
    const a: u8 = this.a;
    
    this.a, const c1: u1 = @subWithOverflow(this.a, this.data);
    this.a, const c2: u1 = @subWithOverflow(this.a, ~this.p.flags.carry);
    
    this.p.flags.carry = ~(c1 | c2);
    this.p.flags.zero = @intFromBool(this.a == 0x00);
    this.p.flags.over = @intFromBool(((this.a ^ a) & (this.a ^ ~this.data) & 0x80) == 0x80);
    this.p.flags.neg  = @intFromBool(this.a >= 0x80);
}

pub fn TAX(this: *NES) void {
    this.x = this.a;

    this.p.flags.zero  = @intFromBool(this.x == 0x00);
    this.p.flags.neg   = @intFromBool(this.x >= 0x80);
}

pub fn TAY(this: *NES) void {
    this.y = this.a;

    this.p.flags.zero  = @intFromBool(this.y == 0x00);
    this.p.flags.neg   = @intFromBool(this.y >= 0x80);
}

pub fn TSX(this: *NES) void {
    this.x = this.sp;

    this.p.flags.zero  = @intFromBool(this.x == 0x00);
    this.p.flags.neg   = @intFromBool(this.x >= 0x80);
}

pub fn TXA(this: *NES) void {
    this.a = this.x;

    this.p.flags.zero  = @intFromBool(this.a == 0x00);
    this.p.flags.neg   = @intFromBool(this.a >= 0x80);
}

pub fn TXS(this: *NES) void {
    this.sp = this.x;
}

pub fn TYA(this: *NES) void {
    this.a = this.y;

    this.p.flags.zero  = @intFromBool(this.a == 0x00);
    this.p.flags.neg   = @intFromBool(this.a >= 0x80);
}