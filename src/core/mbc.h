#pragma once

#include "types.h"

class MBC
{
public:
    MBC(u8* rom, u32 romSize, u8* ram, u32 ramSize);

    virtual void reset() {}

    virtual u8 read8(u16 addr) = 0;
    virtual void write8(u16 addr, u8 val) = 0;

protected:
    static const u16 RAM_ADDR = 0xA000;

    u8* rom;
    u32 romSize;

    u8* ram;
    u32 ramSize;
};


class MBC_None : public MBC
{
public:
    MBC_None(u8* rom, u32 romSize, u8* ram, u32 ramSize);

    u8 read8(u16 addr) override;
    void write8(u16 addr, u8 val) override;

private:
    u16 ramLimit;
};

class MBC1 : public MBC
{
public:
    MBC1(u8* rom, u32 romSize, u8* ram, u32 ramSize);

    void reset() override;

    u8 read8(u16 addr) override;
    void write8(u16 addr, u8 val) override;

private:
    u8 romBank;
    u8 ramBank;

    // Registers
    bool ramEnable;
    u8 romLoBank;
    u8 romHiRamBank;
    bool mode;          // 0: 7 bit ROM bank, RAM only bank 0; 1: 5 bit ROM bank, 2 bit RAM bank


    u8 numRomBanks;
    u8 numRamBanks;
};
