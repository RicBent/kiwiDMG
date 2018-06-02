#include "mmu.h"
#include "cpu.h"
#include <cstdio>

MMU::MMU() :
    biosLoaded(false)
{
    // TODO: Remove this, should be done in frontend
    loadBios("bios.bin");
}

MMU::~MMU()
{
    unloadBios();
}


void MMU::setComponents(CPU* cpu)
{
    this->cpu = cpu;
}


bool MMU::loadBios(const char* path)
{
    unloadBios();

    FILE* f = fopen(path, "rb");
    if (f == nullptr)
        return false;

    fseek(f, 0, SEEK_END);
    biosSize = static_cast<u32>(ftell(f));
    rewind(f);

    bios = new u8[biosSize];
    fread(bios, biosSize, 1, f);
    fclose(f);

    biosLoaded = true;
    printf("BIOS loaded: %s (%d bytes)\n", path, biosSize);
    return true;
}

void MMU::unloadBios()
{
    if (!biosLoaded)
        return;

    biosLoaded = false;
    delete[] bios;
}


u8 MMU::read8(u16 addr)
{
    if (cpu->isBooting && biosLoaded && addr < biosSize)
        return bios[addr];

    if (addr >= 0x8000 && addr <= 0x9FFF)
        return vram[addr & 0x1FFF];

    if (addr >= 0xC000 && addr <= 0xDFFF)
        return wram[addr - 0xC000];

    if (addr >= 0xE000 && addr <= 0xFDFF)
        return wram[addr & 0xFFF];

    if (addr >= 0xFE00 && addr <= 0xFE9F)
        return oam[addr & 0xFF];

    if (addr >= 0xFF00 && addr <= 0xFF7F)
        return io[addr & 0xFF];

    if (addr >= 0xFF80 && addr <= 0xFFFE)
        return hram[addr - 0xFF80];

    printf("Unknown memory read: 0x%04X\n", addr);
    return 0xFF;
}

u16 MMU::read16(u16 addr)
{
    return (read8(addr) | (read8(addr + 1) << 8));
}

void MMU::write8(u16 addr, u8 val)
{
    if (addr >= 0x8000 && addr <= 0x9FFF)
        vram[addr & 0x1FFF] = val;

    else if (addr >= 0xFF00 && addr <= 0xFF7F)
        io[addr & 0xFF] = val;

    else if (addr >= 0xFF80 && addr <= 0xFFFE)
        hram[addr - 0xFF80] = val;

    else
        printf("Unknown memory write: 0x%04X 0x%02X at 0x%04X\n", addr, val, cpu->pc_b);
}

void MMU::write16(u16 addr, u16 val)
{
    write8(addr, val & 0xFF);
    write8(addr + 1, val >> 8);
}