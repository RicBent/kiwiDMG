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

    bool debug = false;

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

    u16 pc_b;   // PC before fetching instruction

    enum Flag
    {
        FLAG_Z = 1 << 7,    // Zero
        FLAG_N = 1 << 6,    // Negative
        FLAG_H = 1 << 5,    // Halfcarry
        FLAG_C = 1 << 4,    // Carry

        FLAGS_NONE = 0,
        FLAGS_ZNHC = FLAG_Z | FLAG_N | FLAG_H | FLAG_C,

        FLAGS_ZNH = FLAG_Z | FLAG_N | FLAG_H,
        FLAGS_ZNC = FLAG_Z | FLAG_N | FLAG_C,
        FLAGS_ZHC = FLAG_Z | FLAG_H | FLAG_C,
        FLAGS_NHC = FLAG_N | FLAG_H | FLAG_C,

        FLAGS_ZN = FLAG_Z | FLAG_N,
        FLAGS_ZH = FLAG_Z | FLAG_H,
        FLAGS_ZC = FLAG_Z | FLAG_C,
        FLAGS_NH = FLAG_N | FLAG_H,
        FLAGS_NC = FLAG_N | FLAG_C,
        FLAGS_HC = FLAG_H | FLAG_C
    };

    // Flag helpers
    bool f_is(Flag f);
    void f_set(Flag f);
    void f_clear(Flag f);
    void f_set(Flag f, bool val);
    void f_clear();


    // Other state variables
    u32 cycles;
    bool isBooting;

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
    void op_ld(u8& reg, u8 val);
    void op_ld_16(u16& reg, u16 val);
    void op_ld_abs(u16 addr, u8 val);
    void op_jr();
    void op_jr(bool cond);
    void op_inc(u8& reg);
    void op_inc_hlp();
    void op_inc_16(u16& reg);
    void op_dec(u8& reg);
    void op_dec_hlp();
    void op_dec_16(u16& reg);
    void op_xor(u8 val);
    void op_cp(u8 val);
    void op_call();
    void op_call(bool cond);
    void op_ret();
    void op_ret(bool cond);
    void op_pop(u16& reg);
    void op_push(u16 val);

    void op_rl(u8& reg, bool rla = false);
    void op_rl_hlp();
    void op_bit(u8 val, u8 bit);
};

// Preprocessor cleanup
#undef DEFINE_REG
