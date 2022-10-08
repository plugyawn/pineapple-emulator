#pragma once
#include "mapper000.h"
#include "mapper.h"

mapper000::mapper000(uint8_t prgBanks, uint8_t chrBanks) :mapper(prgBanks, chrBanks) {}
mapper000::~mapper000() {}

bool mapper000::cpuMapRead(uint16_t addr, uint32_t& mapped_addr) {
    if (addr >= 0x8000 && addr <= 0xFFFF) {
        // These locations can ONLY correspond to the cartridge and nowhere else
        // The address starting from 8000, but the ROM sees it as 0000
        // So we have to transform it.
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF); // THE ACTUAL MAPPING STEP
        // This is because if the PRGBank count is more than 1, it's a 32 KB ROM for sure.
        // In that case, the thing will be mapped to HALF the 16 bit range.
        // addr & 0x3FFF does the job of the mirroring. It is the modulo function for 16 KB mirrored.
        //
        return true;
    }
    return false;
}

bool mapper000::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr) {
    if (addr >= 0x8000 && addr <= 0xFFFF) {
        // These locations can ONLY correspond to the cartridge and nowhere else
        // The address starting from 8000, but the ROM sees it as 0000
        // So we have to transform it.
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF); // THE ACTUAL MAPPING STEP
        // This is because if the PRGBank count is more than 1, it's a 32 KB ROM for sure.
        // In that case, the thing will be mapped to HALF the 16 bit range.
        // addr & 0x3FFF does the job of the mirroring. It is the modulo function for 16 KB mirrored.
        //
        // These locations can ONLY correspond to the cartridge and nowhere else
        return true;
    }
    return false;
    return false;
}

bool mapper000::ppuMapRead(uint16_t addr, uint32_t& mapped_addr) {
    if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		mapped_addr = addr;
		return true;
	}

	return false;
}

bool mapper000::ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) {
    if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		if (nCHRBanks == 0)
		{
			// Treat as RAM
			mapped_addr = addr;
			return true;
		}
	}

	return false;
}

void mapper000::reset()
{

}

