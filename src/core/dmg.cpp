#include "dmg.h"

#include "cpu.h"
#include "mmu.h"

#include <cstdio>

DMG::DMG()
{
    mmu = new MMU();
    cpu = new CPU(mmu);
    mmu->setComponents(cpu);
}

DMG::~DMG()
{
    delete cpu;
    delete mmu;
}

void DMG::reset()
{
    cpu->reset();
}

void DMG::run()
{
    while (!cpu->debug)
        cpu->tick();

    printf("--------------------------------\n");
    cpu->printStatus();
}
