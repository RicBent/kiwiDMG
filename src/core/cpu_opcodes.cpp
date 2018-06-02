#include "cpu.h"

void CPU::op_ld(u8& reg, u8 val)
{
    reg = val;
}

void CPU::op_ld_16(u16& reg, u16 val)
{
    reg = val;
}

void CPU::op_ld_abs(u16 addr, u8 val)
{
    mmu->write8(addr, val);
}

void CPU::op_jr()
{
    regs.pc += read_r8();
}

void CPU::op_jr(bool cond)
{
    if (!cond)
        return;

    cycles += 4;
    op_jr();
}

void CPU::op_inc(u8& reg)
{
    reg++;
    f_set(FLAG_Z, reg == 0);
    f_clear(FLAG_N);
    f_set(FLAG_H, (reg & 0xF) == 0x0);
}

void CPU::op_inc_hlp()
{
    u8 val = mmu->read8(regs.hl);
    op_inc(val);
    mmu->write8(regs.hl, val);
}

void CPU::op_inc_16(u16& reg)
{
    reg++;
}

void CPU::op_dec(u8& reg)
{
    reg--;
    f_set(FLAG_Z, reg == 0);
    f_set(FLAG_N);
    f_set(FLAG_H, (reg & 0xF) == 0xF);
}

void CPU::op_dec_hlp()
{
    u8 val = mmu->read8(regs.hl);
    op_dec(val);
    mmu->write8(regs.hl, val);
}

void CPU::op_dec_16(u16& reg)
{
    reg--;
}

void CPU::op_xor(u8 val)
{
    regs.a ^= val;

    f_clear();
    f_set(FLAG_Z, regs.a == 0);
}

void CPU::op_cp(u8 val)
{
    f_set(FLAG_Z, regs.a == val);
    f_set(FLAG_N);
    f_set(FLAG_H, (regs.a & 0xF) < (val & 0xF));
    f_set(FLAG_C, regs.a < val);
}

void CPU::op_call()
{
    writeStack16(regs.pc);
    regs.pc = read_a16();
}

void CPU::op_call(bool cond)
{
    if (!cond)
        return;

    cycles += 12;
    op_call();
}

void CPU::op_ret()
{
    regs.pc = readStack16();
}

void CPU::op_ret(bool cond)
{
    if (!cond)
        return;

    cycles += 12;
    op_ret();
}

void CPU::op_pop(u16& reg)
{
    reg = readStack16();
}

void CPU::op_push(u16 val)
{
    writeStack16(val);
}

void CPU::op_rl(u8& reg, bool rla)
{
    u8 c = f_is(FLAG_C) ? 1 : 0;
    f_set(FLAG_C, (reg & 0x80) != 0);

    reg <<= 1;
    reg |= c;

    f_clear(FLAGS_NH);

    if (rla)
        f_set(FLAG_Z, reg == 0);
}

void CPU::op_rl_hlp()
{
    u8 val = mmu->read8(regs.hl);
    op_rl(val);
    mmu->write8(regs.hl, val);
}

void CPU::op_bit(u8 val, u8 bit)
{
    f_set(FLAG_Z, (val & (1 << bit)) == 0);
    f_clear(FLAG_N);
    f_set(FLAG_H);
}
