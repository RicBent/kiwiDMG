#pragma once

#include "mmu.h"

// TODO: Convert to class

struct PPU
{
    static const u16 LCD_WIDTH = 160;
    static const u16 LCD_HEIGHT = 144;

    PPU(MMU* muu, CPU* cpu);

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

    u8 lcdControl;  // FF40

    enum LCDC
    {
        LCDC_BG_WINDOW_DISPLAY      = 1 << 0,
        LCDC_OBJ_DISPLAY            = 1 << 1,
        LCDC_OBJ_SIZE               = 1 << 2,
        LCDC_BG_MAP_SELECT          = 1 << 3,
        LCDC_BG_WINDOW_TILE_SELECT  = 1 << 4,
        LCDC_WINDOW_DISPLAY         = 1 << 5,
        LCDC_WINDOW_MAP_SELECT      = 1 << 6,
        LCDC_CONTROL_OPERATION      = 1 << 7

    };

    u8 scrollX;     // FF43
    u8 scrollY;     // FF42

    u8 bgPalette;   // FF47
    u8 objPalette1; // FF48
    u8 objPalette2; // FF49

    typedef u8 GbColor;
    GbColor framebuffer[LCD_WIDTH * LCD_HEIGHT];

    enum OBJ_FLAGS
    {
        OBJ_PALETTE     = 1 << 4,
        OBJ_X_FLIP      = 1 << 5,
        OBJ_Y_FLIP      = 1 << 6,
        OBJ_BG_PRIORITY = 1 << 7
    };


    MMU* mmu;
    CPU* cpu;


    void renderScanline();
};
