#pragma once

#include "mmu.h"

// TODO: Convert to class

struct PPU
{
    static const u16 LCD_WIDTH = 160;
    static const u16 LCD_HEIGHT = 144;

    PPU(MMU* muu);

    void reset();
    bool tick(u8 cycles);


    enum State
    {
        State_HBlank,
        State_VBlank,
        State_OAM,
        State_VRAM
    };

    State state;
    u32 clock;
    u8 scanline;

    u8 scrollX;
    u8 scrollY;

    u8 bgPalette;

    typedef u8 GbColor;
    GbColor framebuffer[LCD_WIDTH * LCD_HEIGHT];


    MMU* mmu;


    void renderScanline();
};
