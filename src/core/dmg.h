#pragma once

#include "cpu.h"
#include "ppu.h"

struct DMG
{
    DMG();
    ~DMG();

    void reset();
    void runToVBlank();

    // TODO: Remove
    u8* getFrame() { return ppu->framebuffer; }

    MMU* mmu;
    CPU* cpu;
    PPU* ppu;
};
