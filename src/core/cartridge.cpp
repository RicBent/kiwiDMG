#include "cartridge.h"

#include <cstdio>


Cartridge::Cartridge() :
    rom(nullptr), ram(nullptr), mbc(nullptr), loaded(false)
{

}

Cartridge::~Cartridge()
{
    unload();
}


bool Cartridge::load(const char* path)
{
    unload();

    // Load File
    FILE* f = fopen(path, "rb");
    if (f == nullptr)
        return false;

    fseek(f, 0, SEEK_END);
    u32 fSize = static_cast<u32>(ftell(f));
    rewind(f);

    if (fSize < 0x150)
    {
        fclose(f);
        printf("ROM has no header\n");
        return false;
    }

    rom = new u8[fSize];
    fread(rom, fSize, 1, f);
    fclose(f);


    u8 cartTypeIdx = rom[0x147];
    u8 romSizeIdx = rom[0x148];
    u8 ramSizeIdx = rom[0x149];


    // Get and check ROM Size
    if (romSizeIdx <= ROM_8MB)
        romSize = 0x8000 << romSizeIdx;
    else if (romSizeIdx == ROM_1_1MB)
        romSize = BANK_SIZE * 72;
    else if (romSizeIdx == ROM_1_2MB)
        romSize = BANK_SIZE * 80;
    else if (romSizeIdx == ROM_1_5MB)
        romSize = BANK_SIZE * 96;
    else
    {
        printf("Unknown ROM size\n");
        return false;
    }

    if (romSize != fSize)
    {
        printf("ROM size mismatch\n");
        goto cartInvalid;
    }


    // Get and check RAM Size
    switch (ramSizeIdx)
    {
        case RAM_NONE:  ramSize = 0; break;
        case RAM_2KB:   ramSize = 2 * 1024; break;
        case RAM_8KB:   ramSize = 8 * 1024; break;
        case RAM_32KB:  ramSize = 32 * 1024; break;
        case RAM_128KB: ramSize = 128 * 1024; break;
        case RAM_64KB:  ramSize = 64 * 1024; break;

        default:
            printf("Unknown RAM size\n");
            goto cartInvalid;
    }

    ram = new u8[ramSize];


    // Set MBC
    switch (cartTypeIdx)
    {
        case CART_ROM_ONLY:
        case CART_ROM_RAM:
        case CART_ROM_RAM_BATT:
            mbc = new MBC_None(rom, romSize, ram, ramSize);
            break;

        case CART_MBC1:
        case CART_MBC1_RAM:
        case CART_MBC1_RAM_BATT:
            mbc = new MBC1(rom, romSize, ram, ramSize);
            break;

        default:
            printf("Unknown cartridge type %d\n", cartTypeIdx);
            goto cartInvalid;
    }

    loaded = true;
    printf("Cartridge loaded: %s (%d bytes)\n", path, fSize);
    return true;

cartInvalid:
    unload();
    return false;
}

void Cartridge::unload()
{
    if (rom)
        delete[] rom;
    if (ram)
        delete[] ram;
    if (mbc)
        delete mbc;

    rom = nullptr;
    ram = nullptr;
    mbc = nullptr;

    loaded = false;
}


u8 Cartridge::read8(u16 addr)
{
    return mbc->read8(addr);
}

void Cartridge::write8(u16 addr, u8 val)
{
    mbc->write8(addr, val);
}
