#include "cpu.h"
#include <cstdio>

CPU::CPU(MMU* mmu) :
    mmu(mmu)
{

}

void CPU::reset()
{
    isBooting = true;
    regs.pc = 0;
}


#define INSTR(nbr, func)                        \
    case nbr:                                   \
        regs.pc += instrInfo[nbr].numBytes;     \
        func;                                   \
        cycles += instrInfo[nbr].cycles;        \
        break

#define INSTR_CB(nbr, func)                     \
    case nbr:                                   \
        regs.pc += instrInfoCB[nbr].numBytes;   \
        func;                                   \
        cycles += instrInfoCB[nbr].cycles;      \
        break


u8 CPU::tick()
{
    cycles = 0;

    // Fetch Opcode
    pc_b = regs.pc;
    u8 opcode = mmu->read8(regs.pc);

    // Execute
    switch (opcode)
    {
        // LD r1,r2
        // r1 = A, B, C, D, E, H, L; r2 = A, B, C, D, E, H, L, (HL), d8
        INSTR(0x06, op_ld(regs.b, read_d8()));
        INSTR(0x0E, op_ld(regs.c, read_d8()));
        INSTR(0x16, op_ld(regs.d, read_d8()));
        INSTR(0x1E, op_ld(regs.e, read_d8()));
        INSTR(0x26, op_ld(regs.h, read_d8()));
        INSTR(0x2E, op_ld(regs.l, read_d8()));
        INSTR(0x3E, op_ld(regs.a, read_d8()));

        INSTR(0x40, op_ld(regs.b, regs.b));
        INSTR(0x41, op_ld(regs.b, regs.c));
        INSTR(0x42, op_ld(regs.b, regs.d));
        INSTR(0x43, op_ld(regs.b, regs.e));
        INSTR(0x44, op_ld(regs.b, regs.h));
        INSTR(0x45, op_ld(regs.b, regs.l));
        INSTR(0x46, op_ld(regs.b, mmu->read8(regs.hl)));
        INSTR(0x47, op_ld(regs.b, regs.a));

        INSTR(0x48, op_ld(regs.c, regs.b));
        INSTR(0x49, op_ld(regs.c, regs.c));
        INSTR(0x4A, op_ld(regs.c, regs.d));
        INSTR(0x4B, op_ld(regs.c, regs.e));
        INSTR(0x4C, op_ld(regs.c, regs.h));
        INSTR(0x4D, op_ld(regs.c, regs.l));
        INSTR(0x4E, op_ld(regs.c, mmu->read8(regs.hl)));
        INSTR(0x4F, op_ld(regs.c, regs.a));

        INSTR(0x50, op_ld(regs.d, regs.b));
        INSTR(0x51, op_ld(regs.d, regs.c));
        INSTR(0x52, op_ld(regs.d, regs.d));
        INSTR(0x53, op_ld(regs.d, regs.e));
        INSTR(0x54, op_ld(regs.d, regs.h));
        INSTR(0x55, op_ld(regs.d, regs.l));
        INSTR(0x56, op_ld(regs.d, mmu->read8(regs.hl)));
        INSTR(0x57, op_ld(regs.d, regs.a));

        INSTR(0x58, op_ld(regs.e, regs.b));
        INSTR(0x59, op_ld(regs.e, regs.c));
        INSTR(0x5A, op_ld(regs.e, regs.d));
        INSTR(0x5B, op_ld(regs.e, regs.e));
        INSTR(0x5C, op_ld(regs.e, regs.h));
        INSTR(0x5D, op_ld(regs.e, regs.l));
        INSTR(0x5E, op_ld(regs.e, mmu->read8(regs.hl)));
        INSTR(0x5F, op_ld(regs.e, regs.a));

        INSTR(0x60, op_ld(regs.h, regs.b));
        INSTR(0x61, op_ld(regs.h, regs.c));
        INSTR(0x62, op_ld(regs.h, regs.d));
        INSTR(0x63, op_ld(regs.h, regs.e));
        INSTR(0x64, op_ld(regs.h, regs.h));
        INSTR(0x65, op_ld(regs.h, regs.l));
        INSTR(0x66, op_ld(regs.h, mmu->read8(regs.hl)));
        INSTR(0x67, op_ld(regs.h, regs.a));

        INSTR(0x68, op_ld(regs.l, regs.b));
        INSTR(0x69, op_ld(regs.l, regs.c));
        INSTR(0x6A, op_ld(regs.l, regs.d));
        INSTR(0x6B, op_ld(regs.l, regs.e));
        INSTR(0x6C, op_ld(regs.l, regs.h));
        INSTR(0x6D, op_ld(regs.l, regs.l));
        INSTR(0x6E, op_ld(regs.l, mmu->read8(regs.hl)));
        INSTR(0x6F, op_ld(regs.l, regs.a));

        INSTR(0x78, op_ld(regs.a, regs.b));
        INSTR(0x79, op_ld(regs.a, regs.c));
        INSTR(0x7A, op_ld(regs.a, regs.d));
        INSTR(0x7B, op_ld(regs.a, regs.e));
        INSTR(0x7C, op_ld(regs.a, regs.h));
        INSTR(0x7D, op_ld(regs.a, regs.l));
        INSTR(0x7E, op_ld(regs.a, mmu->read8(regs.hl)));
        INSTR(0x7F, op_ld(regs.a, regs.a));

        // LD A, r
        // r = (BC), (DE), (HL+), (HL-)
        INSTR(0x0A, op_ld(regs.a, mmu->read8(regs.bc)));
        INSTR(0x1A, op_ld(regs.a, mmu->read8(regs.de)));
        INSTR(0x2A, op_ld(regs.a, mmu->read8(regs.hl++)));
        INSTR(0x3A, op_ld(regs.a, mmu->read8(regs.hl--)));

        // LD r1,r2
        // r1 = (BC), (DE), (HL), (HL+), (HL-); r2 = A
        INSTR(0x02, op_ld_abs(regs.bc, regs.a));
        INSTR(0x12, op_ld_abs(regs.de, regs.a));
        INSTR(0x22, op_ld_abs(regs.hl++, regs.a));
        INSTR(0x32, op_ld_abs(regs.hl--, regs.a));
        INSTR(0x77, op_ld_abs(regs.hl, regs.a));

        // LD (HL),d8
        INSTR(0x36, op_ld_abs(regs.hl, read_d8()));

        // LD n,nn
        // n = BC, DE, HL, SP; nn = 16 bit immediate value
        INSTR(0x01, op_ld_16(regs.bc, read_d16()));
        INSTR(0x11, op_ld_16(regs.de, read_d16()));
        INSTR(0x21, op_ld_16(regs.hl, read_d16()));
        INSTR(0x31, op_ld_16(regs.sp, read_d16()));

        // LDH (a8),A
        INSTR(0xE0, op_ld_abs(0xFF00 + read_a8(), regs.a));

        // LDH A,(a8)
        INSTR(0xF0, op_ld(regs.a, mmu->read8(0xFF00 + read_a8())));

        // LD (C),A
        INSTR(0xE2, op_ld_abs(0xFF00 + regs.c, regs.a));

        // LD A,(C)
        INSTR(0xF2, op_ld(regs.a, mmu->read8(0xFF00 + regs.c)));

        // LD (a16), A
        INSTR(0xEA, op_ld_abs(read_a16(), regs.a));

        // LD A, (a16)
        INSTR(0xFA, op_ld(regs.a, mmu->read8(read_a16())));

        // RLA
        INSTR(0x17, op_rl(regs.a, true));

        // JR
        INSTR(0x18, op_jr());
        INSTR(0x20, op_jr(!f_is(FLAG_Z)));
        INSTR(0x28, op_jr(f_is(FLAG_Z)));
        INSTR(0x30, op_jr(!f_is(FLAG_C)));
        INSTR(0x38, op_jr(f_is(FLAG_C)));

        // INC n
        // n = A, B, C, D, E, H, L, (HL)
        INSTR(0x04, op_inc(regs.b));
        INSTR(0x0C, op_inc(regs.c));
        INSTR(0x14, op_inc(regs.d));
        INSTR(0x1C, op_inc(regs.e));
        INSTR(0x24, op_inc(regs.h));
        INSTR(0x2C, op_inc(regs.l));
        INSTR(0x34, op_inc_hlp());
        INSTR(0x3C, op_inc(regs.a));

        // DEC n
        // n = A, B, C, D, E, H, L, (HL)
        INSTR(0x05, op_dec(regs.b));
        INSTR(0x0D, op_dec(regs.c));
        INSTR(0x15, op_dec(regs.d));
        INSTR(0x1D, op_dec(regs.e));
        INSTR(0x25, op_dec(regs.h));
        INSTR(0x2D, op_dec(regs.l));
        INSTR(0x35, op_dec_hlp());
        INSTR(0x3D, op_dec(regs.a));

        // INC nn
        // nn = BC, DE, HL, SP
        INSTR(0x03, op_inc_16(regs.bc));
        INSTR(0x13, op_inc_16(regs.de));
        INSTR(0x23, op_inc_16(regs.hl));
        INSTR(0x33, op_inc_16(regs.sp));

        // DEC nn
        // nn = BC, DE, HL, SP
        INSTR(0x0B, op_dec_16(regs.bc));
        INSTR(0x1B, op_dec_16(regs.de));
        INSTR(0x2B, op_dec_16(regs.hl));
        INSTR(0x3B, op_dec_16(regs.sp));

        // XOR n
        // n = A, B, C, D, E, H, L, (HL), d8
        INSTR(0xA8, op_xor(regs.b));
        INSTR(0xA9, op_xor(regs.c));
        INSTR(0xAA, op_xor(regs.d));
        INSTR(0xAB, op_xor(regs.e));
        INSTR(0xAC, op_xor(regs.h));
        INSTR(0xAD, op_xor(regs.l));
        INSTR(0xAE, op_xor(mmu->read8(regs.hl)));
        INSTR(0xAF, op_xor(regs.a));
        INSTR(0xEE, op_xor(read_d8()));

        // CP n
        // n = A, B, C, D, E, H, L, (HL), d8
        INSTR(0xB8, op_cp(regs.b));
        INSTR(0xB9, op_cp(regs.c));
        INSTR(0xBA, op_cp(regs.d));
        INSTR(0xBB, op_cp(regs.e));
        INSTR(0xBC, op_cp(regs.h));
        INSTR(0xBD, op_cp(regs.l));
        INSTR(0xBE, op_cp(mmu->read8(regs.hl)));
        INSTR(0xBF, op_cp(regs.a));
        INSTR(0xFE, op_cp(read_d8()));

        // CALL
        INSTR(0xC4, op_call(!f_is(FLAG_Z)));
        INSTR(0xCC, op_call(f_is(FLAG_Z)));
        INSTR(0xCD, op_call());
        INSTR(0xD4, op_call(!f_is(FLAG_C)));
        INSTR(0xDC, op_call(f_is(FLAG_C)));

        // RET
        INSTR(0xC0, op_ret(!f_is(FLAG_Z)));
        INSTR(0xC8, op_ret(f_is(FLAG_Z)));
        INSTR(0xC9, op_ret());
        INSTR(0xD0, op_ret(!f_is(FLAG_C)));
        INSTR(0xD8, op_ret(f_is(FLAG_C)));

        // POP
        INSTR(0xC1, op_pop(regs.bc));
        INSTR(0xD1, op_pop(regs.de));
        INSTR(0xE1, op_pop(regs.hl));
        INSTR(0xF1, op_pop(regs.af));

        // PUSH
        INSTR(0xC5, op_push(regs.bc));
        INSTR(0xD5, op_push(regs.de));
        INSTR(0xE5, op_push(regs.hl));
        INSTR(0xF5, op_push(regs.af));


        case 0xCB:
        {
            opcode = mmu->read8(regs.pc + 1);

            switch (opcode)
            {
                // RL r
                // r = A, B, C, D, E, H, L, (HL)
                INSTR_CB(0x10, op_rl(regs.b));
                INSTR_CB(0x11, op_rl(regs.c));
                INSTR_CB(0x12, op_rl(regs.d));
                INSTR_CB(0x13, op_rl(regs.e));
                INSTR_CB(0x14, op_rl(regs.h));
                INSTR_CB(0x15, op_rl(regs.l));
                INSTR_CB(0x16, op_rl_hlp());
                INSTR_CB(0x17, op_rl(regs.a));

                // BIT b,r
                // b = 0 ... 7; r = A, B, C, D, E, H, L, (HL)
                INSTR_CB(0x40, op_bit(regs.b, 0));
                INSTR_CB(0x41, op_bit(regs.c, 0));
                INSTR_CB(0x42, op_bit(regs.d, 0));
                INSTR_CB(0x43, op_bit(regs.e, 0));
                INSTR_CB(0x44, op_bit(regs.h, 0));
                INSTR_CB(0x45, op_bit(regs.l, 0));
                INSTR_CB(0x46, op_bit(mmu->read8(regs.hl), 0));
                INSTR_CB(0x47, op_bit(regs.a, 0));
                INSTR_CB(0x48, op_bit(regs.b, 1));
                INSTR_CB(0x49, op_bit(regs.c, 1));
                INSTR_CB(0x4A, op_bit(regs.d, 1));
                INSTR_CB(0x4B, op_bit(regs.e, 1));
                INSTR_CB(0x4C, op_bit(regs.h, 1));
                INSTR_CB(0x4D, op_bit(regs.l, 1));
                INSTR_CB(0x4E, op_bit(mmu->read8(regs.hl), 1));
                INSTR_CB(0x4F, op_bit(regs.a, 1));
                INSTR_CB(0x50, op_bit(regs.b, 2));
                INSTR_CB(0x51, op_bit(regs.c, 2));
                INSTR_CB(0x52, op_bit(regs.d, 2));
                INSTR_CB(0x53, op_bit(regs.e, 2));
                INSTR_CB(0x54, op_bit(regs.h, 2));
                INSTR_CB(0x55, op_bit(regs.l, 2));
                INSTR_CB(0x56, op_bit(mmu->read8(regs.hl), 2));
                INSTR_CB(0x57, op_bit(regs.a, 2));
                INSTR_CB(0x58, op_bit(regs.b, 3));
                INSTR_CB(0x59, op_bit(regs.c, 3));
                INSTR_CB(0x5A, op_bit(regs.d, 3));
                INSTR_CB(0x5B, op_bit(regs.e, 3));
                INSTR_CB(0x5C, op_bit(regs.h, 3));
                INSTR_CB(0x5D, op_bit(regs.l, 3));
                INSTR_CB(0x5E, op_bit(mmu->read8(regs.hl), 3));
                INSTR_CB(0x5F, op_bit(regs.a, 3));
                INSTR_CB(0x60, op_bit(regs.b, 4));
                INSTR_CB(0x61, op_bit(regs.c, 4));
                INSTR_CB(0x62, op_bit(regs.d, 4));
                INSTR_CB(0x63, op_bit(regs.e, 4));
                INSTR_CB(0x64, op_bit(regs.h, 4));
                INSTR_CB(0x65, op_bit(regs.l, 4));
                INSTR_CB(0x66, op_bit(mmu->read8(regs.hl), 4));
                INSTR_CB(0x67, op_bit(regs.a, 4));
                INSTR_CB(0x68, op_bit(regs.b, 5));
                INSTR_CB(0x69, op_bit(regs.c, 5));
                INSTR_CB(0x6A, op_bit(regs.d, 5));
                INSTR_CB(0x6B, op_bit(regs.e, 5));
                INSTR_CB(0x6C, op_bit(regs.h, 5));
                INSTR_CB(0x6D, op_bit(regs.l, 5));
                INSTR_CB(0x6E, op_bit(mmu->read8(regs.hl), 5));
                INSTR_CB(0x6F, op_bit(regs.a, 5));
                INSTR_CB(0x70, op_bit(regs.b, 6));
                INSTR_CB(0x71, op_bit(regs.c, 6));
                INSTR_CB(0x72, op_bit(regs.d, 6));
                INSTR_CB(0x73, op_bit(regs.e, 6));
                INSTR_CB(0x74, op_bit(regs.h, 6));
                INSTR_CB(0x75, op_bit(regs.l, 6));
                INSTR_CB(0x76, op_bit(mmu->read8(regs.hl), 6));
                INSTR_CB(0x77, op_bit(regs.a, 6));
                INSTR_CB(0x78, op_bit(regs.b, 7));
                INSTR_CB(0x79, op_bit(regs.c, 7));
                INSTR_CB(0x7A, op_bit(regs.d, 7));
                INSTR_CB(0x7B, op_bit(regs.e, 7));
                INSTR_CB(0x7C, op_bit(regs.h, 7));
                INSTR_CB(0x7D, op_bit(regs.l, 7));
                INSTR_CB(0x7E, op_bit(mmu->read8(regs.hl), 7));
                INSTR_CB(0x7F, op_bit(regs.a, 7));

                default:
                    printf("ERROR: Unknown CB opcode %02X\n", opcode);
                    debug = true;
                    break;
            }

            break;
        }

        default:
            printf("ERROR: Unknown opcode %02X\n", opcode);
            debug = true;
            break;
    }

    if (isBooting && regs.pc >= mmu->biosSize)
    {
        isBooting = false;

        printf("Booting done! PC: %04X\n", regs.pc);
        debug = true;
    }

    return cycles;
}

void CPU::printStatus()
{
    printf("A: %02X, B: %02X, C: %02X, D: %02X, E: %02X\n", regs.a, regs.b, regs.c, regs.d, regs.e);
    printf("BC: %04X, DE: %04X, HL: %04X, SP: %04X, PC: %04X\n", regs.bc, regs.de, regs.hl, regs.sp, regs.pc);
    printf("Z: %d, N: %d, H: %d, C: %d\n", f_is(FLAG_Z), f_is(FLAG_N), f_is(FLAG_H), f_is(FLAG_C));
}


bool CPU::f_is(Flag f)
{
    return (regs.f & f) != 0;
}

void CPU::f_set(Flag f)
{
    regs.f |= f;
}

void CPU::f_clear(Flag f)
{
    regs.f &= ~f;
}

void CPU::f_set(Flag f, bool val)
{
    if (val) f_set(f);
    else     f_clear(f);
}

void CPU::f_clear()
{
    regs.f = 0;
}

u8 CPU::read_d8()
{
    return mmu->read8(pc_b + 1);
}

u16 CPU::read_d16()
{
    return mmu->read16(pc_b + 1);
}

u8 CPU::read_a8()
{
    return mmu->read8(pc_b + 1);
}

u16 CPU::read_a16()
{
    return mmu->read16(pc_b + 1);
}

s8 CPU::read_r8()
{
    return static_cast<s8>(mmu->read8(pc_b + 1));
}

u16 CPU::readStack16()
{
    u16 val = mmu->read16(regs.sp);
    regs.sp += 2;
    return val;
}

void CPU::writeStack16(u16 val)
{
    regs.sp -= 2;
    mmu->write16(regs.sp, val);
}
