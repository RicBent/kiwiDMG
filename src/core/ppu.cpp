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

void PPU::renderScanline()
{
    u16 mapAddr   = 0x1800;     // TODO: 9800 or 9C00
    u16 tilesAddr = 0x0000;     // TODO: 8000 or 8800

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

        u8 tileId = mmu->vram[mapAddr + mapIdx];    // TODO: s8 if tileAddr = 8800

        u16 tileAddr = tilesAddr + tileId * 16 + tileY * 2;

        u8 tileLo = mmu->vram[tileAddr];
        u8 tileHi = mmu->vram[tileAddr + 1];

        for (tileX; tileX < 8; tileX++)
        {
            u8 palIdx = ((tileLo & 0x80) | ((tileHi & 0x80) << 1)) >> 7;
            GbColor c = (bgPalette >> (palIdx * 2)) & 3;

            framebuffer[drawX + scanline * LCD_WIDTH] = c;

            drawX++;
            if (drawX >= LCD_WIDTH)
                return;

            tileLo <<= 1;
            tileHi <<= 1;
        }

        // Next tile
        tileX = 0;
        mapX++;
    }
}
