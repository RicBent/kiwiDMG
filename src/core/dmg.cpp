#include "dmg.h"

#include <cstdio>

DMG::DMG()
{
    mmu = new MMU();
    cpu = new CPU(mmu);
    ppu = new PPU(mmu);

    mmu->setComponents(cpu, ppu);
}

DMG::~DMG()
{
    delete ppu;
    delete cpu;
    delete mmu;
}

void DMG::reset()
{
    mmu->reset();
    cpu->reset();
    ppu->reset();
}

void DMG::runToVBlank()
{
    while (true)
    {
        if (cpu->debugBreak)
            break;

        u8 cycles = cpu->tick();

        if (cpu->debugBreak)
        {
            printf("------------------------\n");
            cpu->printStatus();
            break;
        }

        bool vblank = ppu->tick(cycles);

        if (vblank)
            break;
    }
}
