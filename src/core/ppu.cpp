#include "ppu.h"
#include "cpu.h"

PPU::PPU(MMU* mmu, CPU* cpu) :
    mmu(mmu), cpu(cpu)
{

}

void PPU::reset()
{
    clock = 0;
    state = State_HBlank;

    scrollX = 0;
    scrollY = 0;
}

bool PPU::tick(u8 cycles)
{
    clock += cycles;

    bool enteringVBlank = false;

    switch (state)
    {
    case State_HBlank:
        if (clock >= 204)
        {
            clock -= 204;
            scanline++;

            if (scanline >= 144)
            {
                state = State_VBlank;
                cpu->iflags |= CPU::INTERR_VBLANK;
                enteringVBlank = true;
            }
            else
                state = State_OAM;
        }
        break;

    case State_VBlank:
        if (clock >= 456)
        {
            clock -= 456;
            scanline++;

            if (scanline >= 154)
            {
                state = State_OAM;
                scanline = 0;
            }
        }
        break;

    case State_OAM:
        if (clock >= 80)
        {
            clock -= 80;
            state = State_VRAM;
        }

    case State_VRAM:
        if (clock >= 172)
        {
            clock -= 172;
            state = State_HBlank;

            renderScanline();
        }
        break;
    }

    return enteringVBlank;
}


#include <iostream>

void PPU::renderScanline()
{
    // Render BG Map

    // TODO: Tile flipping
    // TODO: Map Select

    if (lcdControl & LCDC_BG_WINDOW_DISPLAY)
    {
        u16 mapAddr = 0x1800;     // TODO: 9800 or 9C00

        u16 mapX, mapY, mapIdx;
        mapY = ((scrollY + scanline) / 8) % 0x20;
        mapX = scrollX / 8;

        u8 tileX, tileY;
        tileX = scrollX % 8;
        tileY = (scrollY + scanline) % 8;

        u8 drawX = 0;
        while (true)
        {
            mapX %= 0x20;
            mapIdx = mapY * 0x20 + mapX;

            u8 tileId = mmu->vram[mapAddr + mapIdx];

            // LCDC_BG_WINDOW_TILE_SELECT
            // 0: Tile address 0x9000, signed index
            // 1: Tile address 0x8000, unsigned index
            u16 tileAddr;
            if (lcdControl & LCDC_BG_WINDOW_TILE_SELECT)
                tileAddr = tileId * 16;
            else
                tileAddr = 0x1000 + static_cast<s8>(tileId) * 16;

            tileAddr += tileY * 2;


            u8 tileLo = mmu->vram[tileAddr];
            u8 tileHi = mmu->vram[tileAddr + 1];

            tileLo <<= tileX;
            tileHi <<= tileX;
            for (tileX; tileX < 8; tileX++)
            {
                u8 palIdx = ((tileLo & 0x80) | ((tileHi & 0x80) << 1)) >> 7;
                GbColor c = (bgPalette >> (palIdx * 2)) & 3;

                framebuffer[drawX + scanline * LCD_WIDTH] = c;

                drawX++;
                if (drawX >= LCD_WIDTH)
                    goto renderObjects;

                tileLo <<= 1;
                tileHi <<= 1;
            }

            // Next tile
            tileX = 0;
            mapX++;
        }
    }

    // Render Objects
renderObjects:

    // TODO: Priorities (lower x first, same x: lower id)
    // TODO: 8x16 sprites
    // TODO: BG Priority
    // TODO: Tile flipping
    if (lcdControl & LCDC_OBJ_DISPLAY)
    {
        u16 oamAddr = 0;
        for (int s = 0; s < 40; s++)
        {
            s16 y = mmu->oam[oamAddr++] - 0x10;
            s16 x = mmu->oam[oamAddr++] - 0x08;
            u8 tileId = mmu->oam[oamAddr++];
            u8 flags = mmu->oam[oamAddr++];

            if (scanline >= y && scanline < (y + 8))
            {
                u16 tileAddr = tileId * 16;
                tileAddr += (scanline-y) * 2;

                u8 tileLo = mmu->vram[tileAddr];
                u8 tileHi = mmu->vram[tileAddr + 1];

                u8 pal = (flags & OBJ_PALETTE) ? objPalette2 : objPalette1;


                u8 tileX = (x < 0) ? -x : 0;
                tileLo <<= tileX;
                tileHi <<= tileX;

                u8 drawX = x + tileX;
                for (tileX; tileX < 8; tileX++)
                {
                    u8 palIdx = (((tileLo & 0x80) | ((tileHi & 0x80) << 1)) >> 7);

                    if (palIdx != 0)
                    {
                        GbColor c = (pal >> (palIdx * 2)) & 3;
                        framebuffer[scanline * LCD_WIDTH + drawX] = c;
                    }

                    if (++drawX >= LCD_WIDTH)
                        break;

                    tileLo <<= 1;
                    tileHi <<= 1;
                }
            }
        }
    }
}