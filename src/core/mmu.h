#pragma once

#include "types.h"

struct CPU;

// TODO: Convert to class

struct MMU
{
    MMU();
    ~MMU();

    void setComponents(CPU* cpu);

    bool loadBios(const char* path);
    void unloadBios();

    u8 read8(u16 addr);
    u16 read16(u16 addr);

    void write8(u16 addr, u8 val);
    void write16(u16 addr, u16 val);


    CPU* cpu;

    // ROM Bank 0       // 0000-3FFF
    // ROM Banks 1-n    // 4000-7FFF
    u8 vram[0x2000];    // 8000-9FFF
    // External RAM     // A000-BFFF
    u8 wram[0x2000];    // C000-DFFF
    // Echo RAM         // E000-FDFF
    u8 oam[0xA0];       // FE00-FE9F
    // Unusable         // FEA0-FEFF
    u8 io[0x80];        // FF00-FF7F
    u8 hram[0x80];      // FF80-FFFE

    bool biosLoaded;
    u8* bios;
    u32 biosSize;
};