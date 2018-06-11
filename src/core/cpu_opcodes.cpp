#include "cpu.h"

void CPU::op_nop() { }

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

void CPU::op_ld_a16_sp()
{
    mmu->write16(read_a16(), regs.sp);
}

void CPU::op_daa()
{
    u16 res = regs.a;

    if (!f_is(FLAG_N))
    {
        if (f_is(FLAG_H) || (res & 0xF) > 9)
            res += 0x06;

        if (f_is(FLAG_C) || res > 0x9F)
            res += 0x60;
    }
    else
    {
        if (f_is(FLAG_H))
            res = (res - 6) & 0xFF;

        if (f_is(FLAG_C))
            res -= 0x60;
    }

    if (res > 0xFF)
        f_set(FLAG_C, res > 0xFF);

    regs.a = static_cast<u8>(res);
    f_set(FLAG_Z, regs.a == 0);
    f_clear(FLAG_H);
}

void CPU::op_cpl()
{
    regs.a = ~regs.a;
    f_set(FLAG_N);
    f_set(FLAG_H);
}

void CPU::op_scf()
{
    f_set(FLAG_C);
    f_clear(FLAG_N | FLAG_H);
}

void CPU::op_ccf()
{
    f_set(FLAG_C, !f_is(FLAG_C));
    f_clear(FLAG_N | FLAG_H);
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

void CPU::op_add_hlp(u16 val)
{
    u32 res = regs.hl + val;

    f_clear(FLAG_N);
    f_set(FLAG_H, ((regs.hl & 0xFFF) + (val & 0xFFF)) > 0xFFF);
    f_set(FLAG_C, res > 0xFFFF);

    regs.hl = static_cast<u16>(res);
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

void CPU::op_add(u8 val)
{
    f_set(FLAG_H, ((regs.a & 0xF) + (val & 0xF)) > 0xF);

    u16 res = regs.a + val;
    f_set(FLAG_C, res > 0xFF);
    regs.a = static_cast<u8>(res);

    f_set(FLAG_Z, regs.a == 0);
    f_clear(FLAG_N);
}

void CPU::op_adc(u8 val)
{
    f_set(FLAG_H, ((regs.a & 0xF) + (val & 0xF) + f_is(FLAG_C)) > 0xF);

    u16 res = regs.a + val + f_is(FLAG_C);
    f_set(FLAG_C, res > 0xFF);
    regs.a = static_cast<u8>(res);

    f_set(FLAG_Z, regs.a == 0);
    f_clear(FLAG_N);
}

void CPU::op_sub(u8 val)
{
    f_set(FLAG_C, regs.a < val);
    f_set(FLAG_H, (regs.a & 0xF) < (val & 0xF));
    regs.a -= val;
    f_set(FLAG_Z, regs.a == 0);
    f_set(FLAG_N);
}

void CPU::op_sbc(u8 val)
{
    u8 c = f_is(FLAG_C);
    u16 sub = val + c;

    f_set(FLAG_H, (regs.a & 0xF) < ((val & 0xF) + c));
    f_set(FLAG_C, regs.a < sub);
    regs.a -= sub;

    f_set(FLAG_Z, regs.a == 0);
    f_set(FLAG_N);
}

void CPU::op_and(u8 val)
{
    regs.a &= val;

    f_set(FLAG_Z, regs.a == 0);
    f_clear(FLAG_N | FLAG_C);
    f_set(FLAG_H);
}

void CPU::op_xor(u8 val)
{
    regs.a ^= val;

    f_clear();
    f_set(FLAG_Z, regs.a == 0);
}

void CPU::op_or(u8 val)
{
    regs.a |= val;

    f_set(FLAG_Z, regs.a == 0);
    f_clear(FLAG_N | FLAG_H | FLAG_C);
}

void CPU::op_cp(u8 val)
{
    f_set(FLAG_Z, regs.a == val);
    f_set(FLAG_N);
    f_set(FLAG_H, (regs.a & 0xF) < (val & 0xF));
    f_set(FLAG_C, regs.a < val);
}

void CPU::op_jp()
{
    regs.pc = read_a16();
}

void CPU::op_jp(bool cond)
{
    if (!cond)
        return;

    cycles += 4;
    op_jp();
}

void CPU::op_jp_hlp()
{
    regs.pc = regs.hl;
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

void CPU::op_reti()
{
    ime = true;
    op_ret();
}

void CPU::op_pop(u16& reg)
{
    reg = readStack16();
}

void CPU::op_pop_af()
{
    // Only top 4 bits of F may be written
    regs.af = readStack16() & 0xFFF0;
}

void CPU::op_push(u16 val)
{
    writeStack16(val);
}

void CPU::op_rst(u8 addr)
{
    writeStack16(regs.pc);
    regs.pc = addr;
}

void CPU::op_add_sp_r8()
{
    s8 r8 = read_r8();
    s32 result = regs.sp + r8;

    f_clear(FLAG_Z | FLAG_N);
    f_set(FLAG_H, ((regs.sp ^ r8 ^ (result & 0xFFFF)) & 0x10) == 0x10);
    f_set(FLAG_C, ((regs.sp ^ r8 ^ (result & 0xFFFF)) & 0x100) == 0x100);

    regs.sp = static_cast<u16>(result);
}

void CPU::op_ld_sp_r8()
{
    s8 r8 = read_r8();
    s32 result = regs.sp + r8;

    f_clear(FLAG_Z | FLAG_N);
    f_set(FLAG_H, ((regs.sp ^ r8 ^ (result & 0xFFFF)) & 0x10) == 0x10);
    f_set(FLAG_C, ((regs.sp ^ r8 ^ (result & 0xFFFF)) & 0x100) == 0x100);

    regs.hl = static_cast<u16>(result);
}

void CPU::op_ei()
{
    ime = true;
}

void CPU::op_di()
{
    ime = false;
}


void CPU::op_rlc(u8& reg, bool rlca)
{
    u8 rot = (reg & 0x80) ? 1 : 0;
    reg = (reg << 1) | rot;

    f_set(FLAG_C, rot);
    f_clear(FLAG_N | FLAG_H);
    f_set(FLAG_Z, rlca ? false : (reg == 0));
}

void CPU::op_rlc_hlp()
{
    u8 val = mmu->read8(regs.hl);
    op_rlc(val);
    mmu->write8(regs.hl, val);
}

void CPU::op_rrc(u8& reg, bool rrca)
{
    u8 rot = (reg & 1) ? 0x80 : 0;
    reg = (reg >> 1) | rot;

    f_set(FLAG_C, rot);
    f_clear(FLAG_N | FLAG_H);
    f_set(FLAG_Z, rrca ? false : (reg == 0));
}

void CPU::op_rrc_hlp()
{
    u8 val = mmu->read8(regs.hl);
    op_rrc(val);
    mmu->write8(regs.hl, val);
}

void CPU::op_rl(u8& reg, bool rla)
{
    u8 c = f_is(FLAG_C) ? 1 : 0;
    f_set(FLAG_C, (reg & 0x80) != 0);

    reg <<= 1;
    reg |= c;

    f_clear(FLAG_N | FLAG_H);
    f_set(FLAG_Z, rla ? false : (reg == 0));
}

void CPU::op_rl_hlp()
{
    u8 val = mmu->read8(regs.hl);
    op_rl(val);
    mmu->write8(regs.hl, val);
}

void CPU::op_rr(u8& reg, bool rra)
{
    u8 c = f_is(FLAG_C) ? 0x80 : 0;
    f_set(FLAG_C, (reg & 1) != 0);

    reg >>= 1;
    reg |= c;

    f_clear(FLAG_N | FLAG_H);
    f_set(FLAG_Z, rra ? false : (reg == 0));
}

void CPU::op_rr_hlp()
{
    u8 val = mmu->read8(regs.hl);
    op_rr(val);
    mmu->write8(regs.hl, val);
}

void CPU::op_sla(u8& reg)
{
    f_set(FLAG_C, (reg & 0x80) != 0);
    reg <<= 1;
    f_clear(FLAG_N | FLAG_H);
    f_set(FLAG_Z, reg == 0);
}

void CPU::op_sla_hlp()
{
    u8 val = mmu->read8(regs.hl);
    op_sla(val);
    mmu->write8(regs.hl, val);
}

void CPU::op_sra(u8& reg)
{
    u8 msb = reg & 0x80;
    f_set(FLAG_C, (reg & 1) != 0);
    reg = (reg >> 1) | msb;
    f_clear(FLAG_N | FLAG_H);
    f_set(FLAG_Z, reg == 0);
}

void CPU::op_sra_hlp()
{
    u8 val = mmu->read8(regs.hl);
    op_sra(val);
    mmu->write8(regs.hl, val);
}

void CPU::op_swap(u8& reg)
{
    reg = (reg >> 4) | ((reg & 0xF) << 4);
    f_set(FLAG_Z, reg == 0);
    f_clear(FLAG_N | FLAG_H | FLAG_C);
}

void CPU::op_swap_hlp()
{
    u8 val = mmu->read8(regs.hl);
    op_swap(val);
    mmu->write8(regs.hl, val);
}

void CPU::op_srl(u8& reg)
{
    f_set(FLAG_C, reg & 1);
    reg >>= 1;
    f_set(FLAG_Z, reg == 0);
    f_clear(FLAG_N | FLAG_H);
}

void CPU::op_srl_hlp()
{
    u8 val = mmu->read8(regs.hl);
    op_srl(val);
    mmu->write8(regs.hl, val);
}

void CPU::op_bit(u8 val, u8 bit)
{
    f_set(FLAG_Z, (val & (1 << bit)) == 0);
    f_clear(FLAG_N);
    f_set(FLAG_H);
}

void CPU::op_res(u8& reg, u8 bit)
{
    reg &= ~(1 << bit);
}

void CPU::op_res_hlp(u8 bit)
{
    u8 val = mmu->read8(regs.hl);
    op_res(val, bit);
    mmu->write8(regs.hl, val);
}

void CPU::op_set(u8& reg, u8 bit)
{
    reg |= 1 << bit;
}

void CPU::op_set_hlp(u8 bit)
{
    u8 val = mmu->read8(regs.hl);
    op_set(val, bit);
    mmu->write8(regs.hl, val);
}
