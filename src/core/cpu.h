#pragma once

#include "mmu.h"

#define DEFINE_REG(lower, upper)        \
    union                               \
    {                                   \
        struct { u8 upper; u8 lower; }; \
        u16 lower##upper;               \
    }


// TODO: Convert to class

struct CPU
{
    CPU(MMU* mmu);

    void reset();
    u8 tick();
    void printStatus();

    bool debugBreak = false;

    // Registers
    struct
    {
        DEFINE_REG(a, f);
        DEFINE_REG(b, c);
        DEFINE_REG(d, e);
        DEFINE_REG(h, l);

        u16 sp;
        u16 pc;

    } regs;

    // PC before fetching instruction
    u16 pc_b;

    enum Flag
    {
        FLAG_Z = 1 << 7,    // Zero
        FLAG_N = 1 << 6,    // Negative
        FLAG_H = 1 << 5,    // Halfcarry
        FLAG_C = 1 << 4,    // Carry
    };

    // Flag helpers
    bool f_is(u8 f);
    void f_set(u8 f);
    void f_clear(u8 f);
    void f_set(u8 f, bool val);
    void f_clear();

    // Cycles of current instruction
    u8 cycles;

    MMU* mmu;


    // Instruction info tables
    struct InstrInfo
    {
        u8 numBytes;
        u8 cycles;
        const char* mnemonic;
    };

    static const InstrInfo instrInfo[0x100];
    static const InstrInfo instrInfoCB[0x100];


    // Read/write helpers
    u8 read_d8();
    u16 read_d16();
    u8 read_a8();
    u16 read_a16();
    s8 read_r8();

    u16 readStack16();
    void writeStack16(u16 val);

    // Opcodes
    void op_nop();
    void op_ld(u8& reg, u8 val);
    void op_ld_16(u16& reg, u16 val);
    void op_ld_abs(u16 addr, u8 val);
    void op_ld_a16_sp();
    void op_jr();
    void op_jr(bool cond);
    void op_add_hlp(u16 val);
    void op_daa();
    void op_cpl();
    void op_scf();
    void op_ccf();
    void op_inc(u8& reg);
    void op_inc_hlp();
    void op_inc_16(u16& reg);
    void op_dec(u8& reg);
    void op_dec_hlp();
    void op_dec_16(u16& reg);
    void op_add(u8 val);
    void op_adc(u8 val);
    void op_sub(u8 val);
    void op_sbc(u8 val);
    void op_and(u8 val);
    void op_xor(u8 val);
    void op_or(u8 val);
    void op_cp(u8 val);
    void op_jp();
    void op_jp(bool cond);
    void op_jp_hlp();
    void op_call();
    void op_call(bool cond);
    void op_ret();
    void op_ret(bool cond);
    void op_pop(u16& reg);
    void op_pop_af();
    void op_push(u16 val);
    void op_rst(u8 addr);
    void op_add_sp_r8();
    void op_ld_sp_r8();

    // Opcodes (CB prefixed)
    void op_rlc(u8& reg, bool rlca = false);
    void op_rlc_hlp();
    void op_rrc(u8& reg, bool rrca = false);
    void op_rrc_hlp();
    void op_rl(u8& reg, bool rla = false);
    void op_rl_hlp();
    void op_rr(u8& reg, bool rra = false);
    void op_rr_hlp();
    void op_sla(u8& reg);
    void op_sla_hlp();
    void op_sra(u8& reg);
    void op_sra_hlp();
    void op_swap(u8& reg);
    void op_swap_hlp();
    void op_srl(u8& reg);
    void op_srl_hlp();
    void op_bit(u8 val, u8 bit);
    void op_res(u8& reg, u8 bit);
    void op_res_hlp(u8 bit);
    void op_set(u8& reg, u8 bit);
    void op_set_hlp(u8 bit);
};

// Preprocessor cleanup
#undef DEFINE_REG
