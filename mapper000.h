#pragma once
#include "mapper.h"

class mapper000 : public mapper {
public:
    mapper000(uint8_t prgBanks, uint8_t chrBanks);
    ~mapper000();
public:
    bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
    bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr) override;
    bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
    bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) override;

    void reset() override;
};