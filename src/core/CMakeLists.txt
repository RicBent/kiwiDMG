add_library(core STATIC
        types.h
        dmg.cpp
        dmg.h
        mmu.cpp
        mmu.h
        cpu.cpp
        cpu.h
        cpu_opcodes.cpp
        cpu_instinfo.cpp
        cpu_instrinfo_cb.cpp
        ppu.cpp
        ppu.h
        cartridge.cpp
        cartridge.h
        mbc.cpp
        mbc.h)
