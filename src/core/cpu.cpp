#include "cpu.h"
#include <cstdio>

CPU::CPU(MMU* mmu) :
    mmu(mmu)
{

}

void CPU::reset()
{
    debugBreak = false;
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
        // Hacks
        INSTR(0xF3, op_nop()); // DI
        INSTR(0xFB, op_nop()); // EI
        INSTR(0xD9, op_ret()); // RETI

        // NOP
        INSTR(0x00, op_nop());

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

        // LD (HL),r
        // r = A, B, C, D, E, H, L
        INSTR(0x70, op_ld_abs(regs.hl, regs.b));
        INSTR(0x71, op_ld_abs(regs.hl, regs.c));
        INSTR(0x72, op_ld_abs(regs.hl, regs.d));
        INSTR(0x73, op_ld_abs(regs.hl, regs.e));
        INSTR(0x74, op_ld_abs(regs.hl, regs.h));
        INSTR(0x75, op_ld_abs(regs.hl, regs.l));
        INSTR(0x77, op_ld_abs(regs.hl, regs.a));

        // LD r1,r2
        // r1 = (BC), (DE), (HL+), (HL-); r2 = A
        INSTR(0x02, op_ld_abs(regs.bc, regs.a));
        INSTR(0x12, op_ld_abs(regs.de, regs.a));
        INSTR(0x22, op_ld_abs(regs.hl++, regs.a));
        INSTR(0x32, op_ld_abs(regs.hl--, regs.a));

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

        // LD SP,HL
        INSTR(0xF9, op_ld_16(regs.sp, regs.hl));

        // LD (a16), SP
        INSTR(0x08, op_ld_a16_sp());

        // RLCA
        INSTR(0x07, op_rlc(regs.a, true));

        // RRCA
        INSTR(0x0F, op_rrc(regs.a, true));

        // RLA
        INSTR(0x17, op_rl(regs.a, true));

        // RRA
        INSTR(0x1F, op_rr(regs.a, true));

        // CPL
        INSTR(0x27, op_daa());

        // CPL
        INSTR(0x2F, op_cpl());

        // SCF
        INSTR(0x37, op_scf());

        // SCF
        INSTR(0x3F, op_ccf());

        // JR
        INSTR(0x18, op_jr());
        INSTR(0x20, op_jr(!f_is(FLAG_Z)));
        INSTR(0x28, op_jr(f_is(FLAG_Z)));
        INSTR(0x30, op_jr(!f_is(FLAG_C)));
        INSTR(0x38, op_jr(f_is(FLAG_C)));

        // ADD HL, r
        // r = BC, DE, HL, SP
        INSTR(0x09, op_add_hlp(regs.bc));
        INSTR(0x19, op_add_hlp(regs.de));
        INSTR(0x29, op_add_hlp(regs.hl));
        INSTR(0x39, op_add_hlp(regs.sp));

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

        // ADD n
        // n = A, B, C, D, E, H, L, (HL), d8
        INSTR(0x80, op_add(regs.b));
        INSTR(0x81, op_add(regs.c));
        INSTR(0x82, op_add(regs.d));
        INSTR(0x83, op_add(regs.e));
        INSTR(0x84, op_add(regs.h));
        INSTR(0x85, op_add(regs.l));
        INSTR(0x86, op_add(mmu->read8(regs.hl)));
        INSTR(0x87, op_add(regs.a));
        INSTR(0xC6, op_add(read_d8()));

        // ADC n
        // n = A, B, C, D, E, H, L, (HL), d8
        INSTR(0x88, op_adc(regs.b));
        INSTR(0x89, op_adc(regs.c));
        INSTR(0x8A, op_adc(regs.d));
        INSTR(0x8B, op_adc(regs.e));
        INSTR(0x8C, op_adc(regs.h));
        INSTR(0x8D, op_adc(regs.l));
        INSTR(0x8E, op_adc(mmu->read8(regs.hl)));
        INSTR(0x8F, op_adc(regs.a));
        INSTR(0xCE, op_adc(read_d8()));

        // SUB n
        // n = A, B, C, D, E, H, L, (HL), d8
        INSTR(0x90, op_sub(regs.b));
        INSTR(0x91, op_sub(regs.c));
        INSTR(0x92, op_sub(regs.d));
        INSTR(0x93, op_sub(regs.e));
        INSTR(0x94, op_sub(regs.h));
        INSTR(0x95, op_sub(regs.l));
        INSTR(0x96, op_sub(mmu->read8(regs.hl)));
        INSTR(0x97, op_sub(regs.a));
        INSTR(0xD6, op_sub(read_d8()));

        // SBC n
        // n = A, B, C, D, E, H, L, (HL), d8
        INSTR(0x98, op_sbc(regs.b));
        INSTR(0x99, op_sbc(regs.c));
        INSTR(0x9A, op_sbc(regs.d));
        INSTR(0x9B, op_sbc(regs.e));
        INSTR(0x9C, op_sbc(regs.h));
        INSTR(0x9D, op_sbc(regs.l));
        INSTR(0x9E, op_sbc(mmu->read8(regs.hl)));
        INSTR(0x9F, op_sbc(regs.a));
        INSTR(0xDE, op_sbc(read_d8()));

        // AND n
        // n = A, B, C, D, E, H, L, (HL), d8
        INSTR(0xA0, op_and(regs.b));
        INSTR(0xA1, op_and(regs.c));
        INSTR(0xA2, op_and(regs.d));
        INSTR(0xA3, op_and(regs.e));
        INSTR(0xA4, op_and(regs.h));
        INSTR(0xA5, op_and(regs.l));
        INSTR(0xA6, op_and(mmu->read8(regs.hl)));
        INSTR(0xA7, op_and(regs.a));
        INSTR(0xE6, op_and(read_d8()));

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

        // OR n
        // n = A, B, C, D, E, H, L, (HL), d8
        INSTR(0xB0, op_or(regs.b));
        INSTR(0xB1, op_or(regs.c));
        INSTR(0xB2, op_or(regs.d));
        INSTR(0xB3, op_or(regs.e));
        INSTR(0xB4, op_or(regs.h));
        INSTR(0xB5, op_or(regs.l));
        INSTR(0xB6, op_or(mmu->read8(regs.hl)));
        INSTR(0xB7, op_or(regs.a));
        INSTR(0xF6, op_or(read_d8()));

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

        // JP
        INSTR(0xC2, op_jp(!f_is(FLAG_Z)));
        INSTR(0xC3, op_jp());
        INSTR(0xCA, op_jp(f_is(FLAG_Z)));
        INSTR(0xD2, op_jp(!f_is(FLAG_C)));
        INSTR(0xDA, op_jp(f_is(FLAG_C)));
        INSTR(0xE9, op_jp_hlp());

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
        INSTR(0xF1, op_pop_af());

        // PUSH
        INSTR(0xC5, op_push(regs.bc));
        INSTR(0xD5, op_push(regs.de));
        INSTR(0xE5, op_push(regs.hl));
        INSTR(0xF5, op_push(regs.af));

        // RST
        INSTR(0xC7, op_rst(0x00));
        INSTR(0xCF, op_rst(0x08));
        INSTR(0xD7, op_rst(0x10));
        INSTR(0xDF, op_rst(0x18));
        INSTR(0xE7, op_rst(0x20));
        INSTR(0xEF, op_rst(0x28));
        INSTR(0xF7, op_rst(0x30));
        INSTR(0xFF, op_rst(0x38));

        // ADD SP.r8
        INSTR(0xE8, op_add_sp_r8());

        // LD HL,SP+r8
        INSTR(0xF8, op_ld_sp_r8());


        case 0xCB:
        {
            // Fetch CB opcode
            opcode = mmu->read8(regs.pc + 1);

            switch (opcode)
            {
                // RLC r
                // r = A, B, C, D, E, H, L, (HL)
                INSTR_CB(0x00, op_rlc(regs.b));
                INSTR_CB(0x01, op_rlc(regs.c));
                INSTR_CB(0x02, op_rlc(regs.d));
                INSTR_CB(0x03, op_rlc(regs.e));
                INSTR_CB(0x04, op_rlc(regs.h));
                INSTR_CB(0x05, op_rlc(regs.l));
                INSTR_CB(0x06, op_rlc_hlp());
                INSTR_CB(0x07, op_rlc(regs.a));

                // RRC r
                // r = A, B, C, D, E, H, L, (HL)
                INSTR_CB(0x08, op_rrc(regs.b));
                INSTR_CB(0x09, op_rrc(regs.c));
                INSTR_CB(0x0A, op_rrc(regs.d));
                INSTR_CB(0x0B, op_rrc(regs.e));
                INSTR_CB(0x0C, op_rrc(regs.h));
                INSTR_CB(0x0D, op_rrc(regs.l));
                INSTR_CB(0x0E, op_rrc_hlp());
                INSTR_CB(0x0F, op_rrc(regs.a));

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

                // RR r
                // r = A, B, C, D, E, H, L, (HL)
                INSTR_CB(0x18, op_rr(regs.b));
                INSTR_CB(0x19, op_rr(regs.c));
                INSTR_CB(0x1A, op_rr(regs.d));
                INSTR_CB(0x1B, op_rr(regs.e));
                INSTR_CB(0x1C, op_rr(regs.h));
                INSTR_CB(0x1D, op_rr(regs.l));
                INSTR_CB(0x1E, op_rr_hlp());
                INSTR_CB(0x1F, op_rr(regs.a));

                // SLA r
                // r = A, B, C, D, E, H, L, (HL)
                INSTR_CB(0x20, op_sla(regs.b));
                INSTR_CB(0x21, op_sla(regs.c));
                INSTR_CB(0x22, op_sla(regs.d));
                INSTR_CB(0x23, op_sla(regs.e));
                INSTR_CB(0x24, op_sla(regs.h));
                INSTR_CB(0x25, op_sla(regs.l));
                INSTR_CB(0x26, op_sla_hlp());
                INSTR_CB(0x27, op_sla(regs.a));

                // SRA r
                // r = A, B, C, D, E, H, L, (HL)
                INSTR_CB(0x28, op_sra(regs.b));
                INSTR_CB(0x29, op_sra(regs.c));
                INSTR_CB(0x2A, op_sra(regs.d));
                INSTR_CB(0x2B, op_sra(regs.e));
                INSTR_CB(0x2C, op_sra(regs.h));
                INSTR_CB(0x2D, op_sra(regs.l));
                INSTR_CB(0x2E, op_sra_hlp());
                INSTR_CB(0x2F, op_sra(regs.a));

                // SWAP r
                // r = A, B, C, D, E, H, L, (HL)
                INSTR_CB(0x30, op_swap(regs.b));
                INSTR_CB(0x31, op_swap(regs.c));
                INSTR_CB(0x32, op_swap(regs.d));
                INSTR_CB(0x33, op_swap(regs.e));
                INSTR_CB(0x34, op_swap(regs.h));
                INSTR_CB(0x35, op_swap(regs.l));
                INSTR_CB(0x36, op_swap_hlp());
                INSTR_CB(0x37, op_swap(regs.a));

                // SRL r
                // r = A, B, C, D, E, H, L, (HL)
                INSTR_CB(0x38, op_srl(regs.b));
                INSTR_CB(0x39, op_srl(regs.c));
                INSTR_CB(0x3A, op_srl(regs.d));
                INSTR_CB(0x3B, op_srl(regs.e));
                INSTR_CB(0x3C, op_srl(regs.h));
                INSTR_CB(0x3D, op_srl(regs.l));
                INSTR_CB(0x3E, op_srl_hlp());
                INSTR_CB(0x3F, op_srl(regs.a));

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

                // RES b,r
                // b = 0 ... 7; r = A, B, C, D, E, H, L, (HL)
                INSTR_CB(0x80, op_res(regs.b, 0));
                INSTR_CB(0x81, op_res(regs.c, 0));
                INSTR_CB(0x82, op_res(regs.d, 0));
                INSTR_CB(0x83, op_res(regs.e, 0));
                INSTR_CB(0x84, op_res(regs.h, 0));
                INSTR_CB(0x85, op_res(regs.l, 0));
                INSTR_CB(0x86, op_res_hlp(0));
                INSTR_CB(0x87, op_res(regs.a, 0));
                INSTR_CB(0x88, op_res(regs.b, 1));
                INSTR_CB(0x89, op_res(regs.c, 1));
                INSTR_CB(0x8A, op_res(regs.d, 1));
                INSTR_CB(0x8B, op_res(regs.e, 1));
                INSTR_CB(0x8C, op_res(regs.h, 1));
                INSTR_CB(0x8D, op_res(regs.l, 1));
                INSTR_CB(0x8E, op_res_hlp(1));
                INSTR_CB(0x8F, op_res(regs.a, 1));
                INSTR_CB(0x90, op_res(regs.b, 2));
                INSTR_CB(0x91, op_res(regs.c, 2));
                INSTR_CB(0x92, op_res(regs.d, 2));
                INSTR_CB(0x93, op_res(regs.e, 2));
                INSTR_CB(0x94, op_res(regs.h, 2));
                INSTR_CB(0x95, op_res(regs.l, 2));
                INSTR_CB(0x96, op_res_hlp(2));
                INSTR_CB(0x97, op_res(regs.a, 2));
                INSTR_CB(0x98, op_res(regs.b, 3));
                INSTR_CB(0x99, op_res(regs.c, 3));
                INSTR_CB(0x9A, op_res(regs.d, 3));
                INSTR_CB(0x9B, op_res(regs.e, 3));
                INSTR_CB(0x9C, op_res(regs.h, 3));
                INSTR_CB(0x9D, op_res(regs.l, 3));
                INSTR_CB(0x9E, op_res_hlp(3));
                INSTR_CB(0x9F, op_res(regs.a, 3));
                INSTR_CB(0xA0, op_res(regs.b, 4));
                INSTR_CB(0xA1, op_res(regs.c, 4));
                INSTR_CB(0xA2, op_res(regs.d, 4));
                INSTR_CB(0xA3, op_res(regs.e, 4));
                INSTR_CB(0xA4, op_res(regs.h, 4));
                INSTR_CB(0xA5, op_res(regs.l, 4));
                INSTR_CB(0xA6, op_res_hlp(4));
                INSTR_CB(0xA7, op_res(regs.a, 4));
                INSTR_CB(0xA8, op_res(regs.b, 5));
                INSTR_CB(0xA9, op_res(regs.c, 5));
                INSTR_CB(0xAA, op_res(regs.d, 5));
                INSTR_CB(0xAB, op_res(regs.e, 5));
                INSTR_CB(0xAC, op_res(regs.h, 5));
                INSTR_CB(0xAD, op_res(regs.l, 5));
                INSTR_CB(0xAE, op_res_hlp(5));
                INSTR_CB(0xAF, op_res(regs.a, 5));
                INSTR_CB(0xB0, op_res(regs.b, 6));
                INSTR_CB(0xB1, op_res(regs.c, 6));
                INSTR_CB(0xB2, op_res(regs.d, 6));
                INSTR_CB(0xB3, op_res(regs.e, 6));
                INSTR_CB(0xB4, op_res(regs.h, 6));
                INSTR_CB(0xB5, op_res(regs.l, 6));
                INSTR_CB(0xB6, op_res_hlp(6));
                INSTR_CB(0xB7, op_res(regs.a, 6));
                INSTR_CB(0xB8, op_res(regs.b, 7));
                INSTR_CB(0xB9, op_res(regs.c, 7));
                INSTR_CB(0xBA, op_res(regs.d, 7));
                INSTR_CB(0xBB, op_res(regs.e, 7));
                INSTR_CB(0xBC, op_res(regs.h, 7));
                INSTR_CB(0xBD, op_res(regs.l, 7));
                INSTR_CB(0xBE, op_res_hlp(7));
                INSTR_CB(0xBF, op_res(regs.a, 7));

                // SET b,r
                // b = 0 ... 7; r = A, B, C, D, E, H, L, (HL)
                INSTR_CB(0xC0, op_set(regs.b, 0));
                INSTR_CB(0xC1, op_set(regs.c, 0));
                INSTR_CB(0xC2, op_set(regs.d, 0));
                INSTR_CB(0xC3, op_set(regs.e, 0));
                INSTR_CB(0xC4, op_set(regs.h, 0));
                INSTR_CB(0xC5, op_set(regs.l, 0));
                INSTR_CB(0xC6, op_set_hlp(0));
                INSTR_CB(0xC7, op_set(regs.a, 0));
                INSTR_CB(0xC8, op_set(regs.b, 1));
                INSTR_CB(0xC9, op_set(regs.c, 1));
                INSTR_CB(0xCA, op_set(regs.d, 1));
                INSTR_CB(0xCB, op_set(regs.e, 1));
                INSTR_CB(0xCC, op_set(regs.h, 1));
                INSTR_CB(0xCD, op_set(regs.l, 1));
                INSTR_CB(0xCE, op_set_hlp(1));
                INSTR_CB(0xCF, op_set(regs.a, 1));
                INSTR_CB(0xD0, op_set(regs.b, 2));
                INSTR_CB(0xD1, op_set(regs.c, 2));
                INSTR_CB(0xD2, op_set(regs.d, 2));
                INSTR_CB(0xD3, op_set(regs.e, 2));
                INSTR_CB(0xD4, op_set(regs.h, 2));
                INSTR_CB(0xD5, op_set(regs.l, 2));
                INSTR_CB(0xD6, op_set_hlp(2));
                INSTR_CB(0xD7, op_set(regs.a, 2));
                INSTR_CB(0xD8, op_set(regs.b, 3));
                INSTR_CB(0xD9, op_set(regs.c, 3));
                INSTR_CB(0xDA, op_set(regs.d, 3));
                INSTR_CB(0xDB, op_set(regs.e, 3));
                INSTR_CB(0xDC, op_set(regs.h, 3));
                INSTR_CB(0xDD, op_set(regs.l, 3));
                INSTR_CB(0xDE, op_set_hlp(3));
                INSTR_CB(0xDF, op_set(regs.a, 3));
                INSTR_CB(0xE0, op_set(regs.b, 4));
                INSTR_CB(0xE1, op_set(regs.c, 4));
                INSTR_CB(0xE2, op_set(regs.d, 4));
                INSTR_CB(0xE3, op_set(regs.e, 4));
                INSTR_CB(0xE4, op_set(regs.h, 4));
                INSTR_CB(0xE5, op_set(regs.l, 4));
                INSTR_CB(0xE6, op_set_hlp(4));
                INSTR_CB(0xE7, op_set(regs.a, 4));
                INSTR_CB(0xE8, op_set(regs.b, 5));
                INSTR_CB(0xE9, op_set(regs.c, 5));
                INSTR_CB(0xEA, op_set(regs.d, 5));
                INSTR_CB(0xEB, op_set(regs.e, 5));
                INSTR_CB(0xEC, op_set(regs.h, 5));
                INSTR_CB(0xED, op_set(regs.l, 5));
                INSTR_CB(0xEE, op_set_hlp(5));
                INSTR_CB(0xEF, op_set(regs.a, 5));
                INSTR_CB(0xF0, op_set(regs.b, 6));
                INSTR_CB(0xF1, op_set(regs.c, 6));
                INSTR_CB(0xF2, op_set(regs.d, 6));
                INSTR_CB(0xF3, op_set(regs.e, 6));
                INSTR_CB(0xF4, op_set(regs.h, 6));
                INSTR_CB(0xF5, op_set(regs.l, 6));
                INSTR_CB(0xF6, op_set_hlp(6));
                INSTR_CB(0xF7, op_set(regs.a, 6));
                INSTR_CB(0xF8, op_set(regs.b, 7));
                INSTR_CB(0xF9, op_set(regs.c, 7));
                INSTR_CB(0xFA, op_set(regs.d, 7));
                INSTR_CB(0xFB, op_set(regs.e, 7));
                INSTR_CB(0xFC, op_set(regs.h, 7));
                INSTR_CB(0xFD, op_set(regs.l, 7));
                INSTR_CB(0xFE, op_set_hlp(7));
                INSTR_CB(0xFF, op_set(regs.a, 7));

                default:
                    printf("ERROR: Unknown CB opcode %02X\n", opcode);
                    debugBreak = true;
                    break;
            }

            break;
        }

        default:
            printf("ERROR: Unknown opcode %02X\n", opcode);
            debugBreak = true;
            break;
    }

    return cycles;
}

void CPU::printStatus()
{
    printf("A: %02X, B: %02X, C: %02X, D: %02X, E: %02X\n", regs.a, regs.b, regs.c, regs.d, regs.e);
    printf("BC: %04X, DE: %04X, HL: %04X, SP: %04X, PC: %04X\n", regs.bc, regs.de, regs.hl, regs.sp, regs.pc);
    printf("Z: %d, N: %d, H: %d, C: %d\n", f_is(FLAG_Z), f_is(FLAG_N), f_is(FLAG_H), f_is(FLAG_C));
}


bool CPU::f_is(u8 f)
{
    return (regs.f & f) != 0;
}

void CPU::f_set(u8 f)
{
    regs.f |= f;
}

void CPU::f_clear(u8 f)
{
    regs.f &= ~f;
}

void CPU::f_set(u8 f, bool val)
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
