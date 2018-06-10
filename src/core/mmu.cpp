#include "mmu.h"
#include "cpu.h"
#include "ppu.h"
#include <cstdio>

MMU::MMU() :
    biosLoaded(false), cartridgeLoaded(false)
{
    // TODO: Remove this, should be done in frontend
    loadBios("bios.bin");
}

MMU::~MMU()
{
    unloadBios();
    unloadCartridge();
}


void MMU::setComponents(CPU* cpu, PPU* ppu)
{
    this->cpu = cpu;
    this->ppu = ppu;
}

void MMU::reset()
{
    biosLocked = !biosLoaded;
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

bool MMU::loadCartridge(const char* path)
{
    unloadCartridge();

    cartridge = new Cartridge();
    if (!cartridge->load(path))
    {
        delete cartridge;
        return false;
    }

    cartridgeLoaded = true;
    return true;
}

void MMU::unloadCartridge()
{
    if (!cartridgeLoaded)
        return;

    cartridgeLoaded = false;
    delete cartridge;
}


u8 MMU::read8(u16 addr)
{
    if (!biosLocked && addr < biosSize)
        return bios[addr];

    if (cartridgeLoaded && addr <= 0x7FFF)
        return cartridge->read8(addr);

    if (addr >= 0x8000 && addr <= 0x9FFF)
        return vram[addr & 0x1FFF];

    if (cartridgeLoaded && addr >= 0xA000 && addr <= 0xBFFF)
        return cartridge->read8(addr);

    if (addr >= 0xC000 && addr <= 0xDFFF)
        return wram[addr - 0xC000];

    if (addr >= 0xE000 && addr <= 0xFDFF)
        return wram[addr & 0xFFF];

    if (addr >= 0xFE00 && addr <= 0xFE9F)
        return oam[addr & 0xFF];

    if (addr >= 0xFF00 && addr <= 0xFF7F)
        return readIO(addr);

    if (addr >= 0xFF80 && addr <= 0xFFFE)
        return hram[addr - 0xFF80];

    printf("Unknown memory read: 0x%04X PC: %04x\n", addr, cpu->pc_b);
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

    else if (addr >= 0xC000 && addr <= 0xDFFF)
        wram[addr - 0xC000] = val;

    else if (addr >= 0xFF00 && addr <= 0xFF7F)
        writeIO(addr, val);

    else if (addr >= 0xFF80 && addr <= 0xFFFE)
        hram[addr - 0xFF80] = val;

    else
        printf("Unknown memory write: 0x%04X 0x%02X PC: %04x\n", addr, val, cpu->pc_b);
}

void MMU::write16(u16 addr, u16 val)
{
    write8(addr, val & 0xFF);
    write8(addr + 1, val >> 8);
}

u8 MMU::readIO(u16 addr)
{
    switch (addr)
    {
    case 0xFF42:
        return  ppu->scrollY;
    case 0xFF43:
        return  ppu->scrollX;
    case 0xFF44:    // TODO: Return 0 if screen disabled
        return ppu->scanline;
    case 0xFF47:    // TODO: Check of this returns anything
        return  ppu->bgPalette;

    default:
        printf("Unknown IO read: 0x%04X PC: %04x\n", addr, cpu->pc_b);
        return 0xFF;
    }
}

void MMU::writeIO(u16 addr, u8 val)
{
    switch (addr)
    {
    case 0xFF42:
        ppu->scrollY = val; break;
    case 0xFF43:
        ppu->scrollX = val; break;
    case 0xFF47:
        ppu->bgPalette = val; break;
    case 0xFF50:    // TODO: Check read result
        if (val) biosLocked = true; break;

    default:
        printf("Unknown IO write: 0x%04X 0x%02X PC: %04x\n", addr, val, cpu->pc_b);
        break;
    }
}