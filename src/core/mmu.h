#pragma once

#include "cartridge.h"

struct CPU;
struct PPU;

// TODO: Convert to class

struct MMU
{
    MMU();
    ~MMU();

    void setComponents(CPU* cpu, PPU* ppu);
    void reset();

    bool loadBios(const char* path);
    void unloadBios();

    bool loadCartridge(const char* path);
    void unloadCartridge();

    u8 read8(u16 addr);
    u16 read16(u16 addr);

    void write8(u16 addr, u8 val);
    void write16(u16 addr, u16 val);


    CPU* cpu;
    PPU* ppu;

    // ROM Bank 0       // 0000-3FFF
    // ROM Banks 1-n    // 4000-7FFF
    u8 vram[0x2000];    // 8000-9FFF
    // External RAM     // A000-BFFF
    u8 wram[0x2000];    // C000-DFFF
    // Echo RAM         // E000-FDFF
    u8 oam[0xA0];       // FE00-FE9F
    // Unusable         // FEA0-FEFF
    // IO               // FF00-FF7F
    u8 hram[0x80];      // FF80-FFFE

    bool biosLoaded;
    u8* bios;
    u32 biosSize;

    bool biosLocked;


    bool cartridgeLoaded;
    Cartridge* cartridge;


    u8 readIO(u16 addr);
    void writeIO(u16 addr, u8 val);
};