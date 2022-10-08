#include <cstdint>
#pragma once
#include "pineapple6502.h"
#include "pineapple2C02.h"
#include <array>

#include "cartridge.h"
// // This is the C Standard Integer library; renames the standard types into EXPLICT types.

class bus {
public:
    bus();
    ~bus();

public: // Devices connected to the bus
    pineapple6502 cpu; // Connect the CPU
    pineapple2C02 ppu; // Connect the PPU


    std::array<uint8_t, 2 * 1024> cpuRam{}; // 2 KB of RAM Connected

    std::shared_ptr<cartridge> cart;


public: // Bus read and write functions
    void cpuWrite(uint16_t a, uint8_t d); // no return, 16-bit address for writing and 8-bit data to be written
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false); // returns 8-bit integer, looks for 16-bit address;
    // **COMEBACK** what is bReadOnly?
    // No need to emulate the read-write signal --> implied by which function it's called.
    // We're either writing to the bus, or reading from the bus.

public: // This is the System interface
    void insertCartridge(const std::shared_ptr<cartridge>& cartridge);
    void reset();
    void clock();
    // These functions are the buttons that the user sees. The reset BUTTON is the reset BUTTON on the physical NES
    // The insertCartridge, again, is the physical setting of the cartridge itself

private:
    uint32_t nSystemClockCounter = 0;
    // Checks how many times TOTAL the bus has ticked

};

