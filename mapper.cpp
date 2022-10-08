#include <cstdint>
#include "mapper.h"

mapper::mapper(uint8_t prgBanks, uint8_t chrBanks) {
    nPRGBanks = prgBanks;
    nCHRBanks = chrBanks;
}

mapper::~mapper() {}


void mapper::reset()
{

}

