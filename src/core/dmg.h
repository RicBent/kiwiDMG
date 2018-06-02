#pragma once

class CPU;
class MMU;

class DMG
{
public:
    DMG();
    ~DMG();

    void reset();
    void run();

private:
    CPU* cpu;
    MMU* mmu;
};
