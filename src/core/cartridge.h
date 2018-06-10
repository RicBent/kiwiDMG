#pragma once

#include "mbc.h"

class Cartridge
{
public:
    Cartridge();
    virtual ~Cartridge();

    void reset();

    bool load(const char* path);

    u8 read8(u16 addr);
    void write8(u16 addr, u8 val);

private:

    const u16 BANK_SIZE = 0x4000;

    enum CART_TYPE
    {
        CART_ROM_ONLY             = 0x00,
        CART_MBC1                 = 0x01,
        CART_MBC1_RAM             = 0x02,
        CART_MBC1_RAM_BATT        = 0x03,
        CART_MBC2                 = 0x05,
        CART_MBC2_BATT            = 0x06,
        CART_ROM_RAM              = 0x08,
        CART_ROM_RAM_BATT         = 0x09,
        CART_MMMO1                = 0x0B,
        CART_MMMO1_RAM            = 0x0C,
        CART_MMMO1_RAM_BAT        = 0x0D,
        CART_MBC3_TIMER_BATT      = 0x0F,
        CART_MBC3_TIMER_RAM_BATT  = 0x10,
        CART_MBC3                 = 0x11,
        CART_MBC3_RAM             = 0x12,
        CART_MBC3_RAM_BATT        = 0x13,
        CART_MBC5                 = 0x19,
        CART_MBC5_RAM             = 0x1A,
        CART_MBC5_RAM_BATT        = 0x1B,
        CART_MBC5_RUMBLE          = 0x1C,
        CART_MBC5_RUMBLE_RAM      = 0x1D,
        CART_MBC5_RUMBLE_RAM_BATT = 0x1E,
        CART_MBC6                 = 0x20,
        CART_MBC7                 = 0x22
    };

    enum ROM_SIZE
    {
        ROM_32KB    = 0x00,
        ROM_64KB    = 0x01,
        ROM_128KB   = 0x02,
        ROM_256KB   = 0x03,
        ROM_512KB   = 0x04,
        ROM_1MB     = 0x05,
        ROM_2MB     = 0x06,
        ROM_4MB     = 0x07,
        ROM_8MB     = 0x08,
        ROM_1_1MB   = 0x52,
        ROM_1_2MB   = 0x53,
        ROM_1_5MB   = 0x54
    };

    enum RAM_SIZE
    {
        RAM_NONE    = 0x00,
        RAM_2KB     = 0x01,
        RAM_8KB     = 0x02,
        RAM_32KB    = 0x03,
        RAM_128KB   = 0x04,
        RAM_64KB    = 0x05
    };


    u8* rom;
    u32 romSize;

    u8* ram;
    u32 ramSize;

    MBC* mbc;

    bool loaded;


    void unload();
};
