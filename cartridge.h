#pragma once
#include <cstdint>
#include <vector>
#include "mapper000.h"
#include <memory>

class cartridge {

public:
    cartridge(const std::string& sFileName); // the constructor with the file name for where to take the ROM from
    ~cartridge();

    enum MIRROR { // kinds of name table mirroring
        HORIZONTAL,
        VERTICAL,
        ONESCREEN_LO,
        ONESCREEN_HI,
    } mirror = HORIZONTAL;

private:
    bool bImageValid = false;

    uint8_t nMapperID = 0;
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;
    // The cartridge has some PROGRAM MEMORY
    // and some CHARACTER/PATTERN MEMORY
    std::vector<uint8_t> vPRGMemory; // program memory
    std::vector<uint8_t> vCHRMemory; // character or pattern memory

    std::shared_ptr<mapper> pMapper;
public:
    // Comms with the Main bus
    // "ARE YOU HANDLING THIS READ/WRITE OPERATION?"

    bool cpuRead(uint16_t addr, uint8_t& data);
    bool cpuWrite(uint16_t addr, uint8_t data);

    // Comms with the PPU Bus
    bool ppuRead(uint16_t addr, uint8_t& data);
    bool ppuWrite(uint16_t addr, uint8_t data);
    bool ImageValid();
    void reset();
};

