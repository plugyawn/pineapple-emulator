#include <fstream>
#include "cartridge.h"
#include "mapper000.h"

cartridge::cartridge(const std::string& sFileName) {
    //iNES header format. Check the report for more detailed info on how it is formatted.
    // Roughly, it is agreed to be:
    // 16 bytes of header, containing
    // 0-3 -> (NAME) "NES" followed by the name
    // 4 -> Size of PRG ROM in 16 KB units
    // 5 -> Size of CHR ROM in 8 KB units
    // 6 -> Mapper/mirroring/battery/trainer (??)
    // 7 -> Mapper/VS/Playchoice/NES 2.0 (??)
    // NOTE: for 6 and 7 we have just chosen the output to be MAPPER
    // 8 -> PRG RAM SIZE
    // 9 -> TV system (rarely used)
    // 10 -> TV system (almost never used)
    // 11-15 -> unused padding, filled with ZEROES
    struct sHeader {
        char name[4];
        uint8_t prg_rom_chunks;
        uint8_t chr_rom_chunks;
        uint8_t mapper_1;
        uint8_t mapper_2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
    } header;

    bImageValid = false;

    std::ifstream ifs;
    // ifstream is used for reading files in cpp
    ifs.open(sFileName, std::ifstream::binary);
    // open in binary
    if (ifs.is_open()) { // if the file opens
        ifs.read((char*)&header, sizeof(sHeader));
        // BASICALLY
        // BASICALLY start from the location &header (starting location of the struct), and go on for the
        // size of the header.

        // Now, the next 512 bits are called "training information", if it does NOT contain the mapper
        // So, if the mapper isn't here, then SKIP by 512 bits
        // Why we do & 0x04? NOT SURE. Documentation suggested.
        if (header.mapper_1 & 0x04) { // basically last two bits
            ifs.seekg(512, std::ios_base::cur); // NOT SURE WHAT THIS IS
            // NOT SURE NOT SURE NOT SURE
            // "ios_base is a multipurpose class that serves as the base class for all I/O stream classes"
            // seekg just goes to an arbitrary position in the file
            // ios_base::cur is basically the Current position of the stream position indicator.
            // 512 is the offset
        }

        // Determine Mapper ID expression
        nMapperID = ((header.mapper_2 >> 4) << 4) | (header.mapper_1 >> 4);
        mirror = (header.mapper_1 & 0x01) ? VERTICAL : HORIZONTAL;

        uint8_t nFileType = 1; // Ideally, this would actually "FIND" the fileformat.
        // However, here we'll only handle this case.

        if (nFileType == 0) {}
        if (nFileType == 2) {}
        if (nFileType == 1) {
            nPRGBanks = header.prg_rom_chunks; // HOW MANY BANKS ARE THERE?
            vPRGMemory.resize(nPRGBanks * 16384); // RESIZE THE Memory TO THIS SIZE (each being 16384 units long)
            ifs.read((char*)vPRGMemory.data(), vPRGMemory.size()); // Again, start with where the data starts, and
            // keep reading for as long as the size of the vPRGMemory (resized)
            nCHRBanks = header.chr_rom_chunks;
            if(nCHRBanks == 0){
                vCHRMemory.resize(8192);
            }
            else
            vCHRMemory.resize(nCHRBanks * 8192);
            ifs.read((char*)vCHRMemory.data(), vCHRMemory.size()); // SAME LOGIC AS IN PRGBanks
        }

        // Loading the appropriate mappers (we've only used mapper 000, but the documentation allows for more)
        switch (nMapperID) {
        case 0: pMapper = std::make_shared<mapper000>(nPRGBanks, nCHRBanks);
            break;
            // case 1: (add similar, except for mapper 001 and so on, if ever added in the future.
            // Different cartridges will have different values for mapperID, so different mappers will be loaded.
        }
        bImageValid = true;
        ifs.close();
    }

}
cartridge::~cartridge() {}

bool cartridge::ImageValid()
{
    return bImageValid;
}

bool  cartridge::cpuWrite(uint16_t addr, uint8_t data) {
    uint32_t  mapped_addr = 0; // note that it's 32 bits, instead of the usual 16 bits
    if (pMapper->cpuMapWrite(addr, mapped_addr)) { // ASKS the BUS if the information it requires needs to come
        //from the cartridge. If so, it executes.
        vPRGMemory[mapped_addr] = data; // write step
        return true;
    }
    else // cartridge isn't interested
        return false;// returns a boolean to see if the cart is handling said operation
}

bool cartridge::cpuRead(uint16_t addr, uint8_t& data) { // note that for CpuREAD it is pointer, while for Write it's PBV
    uint32_t  mapped_addr = 0; // note that it's 32 bits, instead of the usual 16 bits
    if (pMapper->cpuMapRead(addr, mapped_addr)) { // ASKS the BUS if the information it requires needs to come
        //from the cartridge. If so, it executes.
        data = vPRGMemory[mapped_addr]; // read step
        return true;
    }
    else // cartridge isn't interested
        return false;
}

bool cartridge::ppuRead(uint16_t addr, uint8_t& data) {
    uint32_t  mapped_addr = 0; // note that it's 32 bits, instead of the usual 16 bits
    if (pMapper->ppuMapRead(addr, mapped_addr)) { // ASKS the BUS if the information it requires needs to come
        //from the cartridge. If so, it executes.
        data = vCHRMemory[mapped_addr]; // write step
        return true;
    }
    else // cartridge isn't interested
        return false;// returns a boolean to see if the cart is handling said operation
}

bool cartridge::ppuWrite(uint16_t addr, uint8_t data) {
    uint32_t  mapped_addr = 0; // note that it's 32 bits, instead of the usual 16 bits
    if (pMapper->ppuMapWrite(addr, mapped_addr)) { // ASKS the BUS if the information it requires needs to come
        //from the cartridge. If so, it executes.
        vCHRMemory[mapped_addr] = data; // write step
        return true;
    }
    else // cartridge isn't interested
        return false;// returns a boolean to see if the cart is handling said operation
}

void cartridge::reset()
{
	// Note: This does not reset the ROM contents,
	// but does reset the mapper.
	if (pMapper != nullptr)
		pMapper->reset();
}