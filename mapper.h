#pragma once
#include <cstdint>
// This is an abstract class
// As an abstract class, it's like the framework for a proper class
// In JAVA, "class x extends Y", Y is an abstract class.

// We will be deriving all future Mappers like Mapper000 from this class

class mapper {
public:
    mapper(uint8_t prgBanks, uint8_t chrBanks);
    virtual ~mapper();

public:
    virtual bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
    virtual bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;
    virtual bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
    virtual bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;

    virtual void reset() = 0;


protected:
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;

};
