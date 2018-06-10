#include "mbc.h"

#include <cstdio>

MBC::MBC(u8* rom, u32 romSize, u8* ram, u32 ramSize) :
    rom(rom), romSize(romSize), ram(ram), ramSize(ramSize) {}


MBC_None::MBC_None(u8* rom, u32 romSize, u8* ram, u32 ramSize) :
    MBC(rom, romSize, ram, ramSize)
{
    ramLimit = RAM_ADDR + ramSize;
}

u8 MBC_None::read8(u16 addr)
{
    if (addr < romSize)
        return rom[addr];

    if (addr >= RAM_ADDR && addr < ramLimit)
        return ram[addr - RAM_ADDR];

    printf("Unknown MBC_None read: %04x\n", addr);
    return 0xFF;
};

void MBC_None::write8(u16 addr, u8 val)
{
    if (addr >= RAM_ADDR && addr < ramLimit)
        ram[addr - RAM_ADDR] = val;

    else
        printf("Unknown MBC_None write: %04x %02x\n", addr, val);
}


MBC1::MBC1(u8* rom, u32 romSize, u8* ram, u32 ramSize) :
    MBC(rom, romSize, ram, ramSize)
{
    numRomBanks = romSize / 0x4000;
    numRomBanks = ramSize / 0x4000;
    reset();
}

void MBC1::reset()
{
    ramEnable = false;
    romLoBank = 0;
    romHiRamBank = 0;
    mode = 0;

    romBank = 0;
    ramBank = 0;
}

u8 MBC1::read8(u16 addr)
{
    if (addr <= 0x3FFF)
        return rom[addr];
    if (addr <= 0x7FFF)
        return rom[addr + romBank*0x4000];
    if (addr >= 0xA000 && addr <= 0xBFFF)
        return ram[addr - 0xA000 + ramBank*2000];

    printf("Unknown MBC1 read: %04x\n", addr);
    return 0xFF;
};

void MBC1::write8(u16 addr, u8 val)
{
    if (addr <= 0x1FFF)
    {
        if (val == 0x00)
            ramEnable = false;
        else if (val == 0x0A)
            ramEnable = true;
    }

    else if (addr <= 0x7FFF)
    {
        if (addr <= 0x3FFF)
        {
            romLoBank = val & 0x1F;
            if (romLoBank == 0)
                romLoBank = 1;
        }
        else if (addr <= 0x5FFF)
            romHiRamBank = val & 3;
        else
            mode = val & 1;

        if (mode == false)
        {
            romBank = romLoBank & (romHiRamBank << 5);
            ramBank = 0;
        }
        else
        {
            romBank = romLoBank;
            ramBank = romHiRamBank;
        }

        // TODO: Check if this is correct. Probably not.
        if (romBank > numRomBanks)
            romBank = 0;

        if (ramBank > numRamBanks)
            romBank = 0;
    }

    else
        printf("Unknown MBC_None write: %04x %02x\n", addr, val);
}