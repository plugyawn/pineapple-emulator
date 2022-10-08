#include "pineapple2C02.h"
#include "bus.h"
#include "olcPixelGameEngine.h"
#pragma once
// Constructor
pineapple2C02::pineapple2C02() {
    // this palette has been copy pasted from OneLoneCoder's github repo
    palScreen[0x00] = olc::Pixel(84, 84, 84);
    palScreen[0x01] = olc::Pixel(0, 30, 116);
    palScreen[0x02] = olc::Pixel(8, 16, 144);
    palScreen[0x03] = olc::Pixel(48, 0, 136);
    palScreen[0x04] = olc::Pixel(68, 0, 100);
    palScreen[0x05] = olc::Pixel(92, 0, 48);
    palScreen[0x06] = olc::Pixel(84, 4, 0);
    palScreen[0x07] = olc::Pixel(60, 24, 0);
    palScreen[0x08] = olc::Pixel(32, 42, 0);
    palScreen[0x09] = olc::Pixel(8, 58, 0);
    palScreen[0x0A] = olc::Pixel(0, 64, 0);
    palScreen[0x0B] = olc::Pixel(0, 60, 0);
    palScreen[0x0C] = olc::Pixel(0, 50, 60);
    palScreen[0x0D] = olc::Pixel(0, 0, 0);
    palScreen[0x0E] = olc::Pixel(0, 0, 0);
    palScreen[0x0F] = olc::Pixel(0, 0, 0);

    palScreen[0x10] = olc::Pixel(152, 150, 152);
    palScreen[0x11] = olc::Pixel(8, 76, 196);
    palScreen[0x12] = olc::Pixel(48, 50, 236);
    palScreen[0x13] = olc::Pixel(92, 30, 228);
    palScreen[0x14] = olc::Pixel(136, 20, 176);
    palScreen[0x15] = olc::Pixel(160, 20, 100);
    palScreen[0x16] = olc::Pixel(152, 34, 32);
    palScreen[0x17] = olc::Pixel(120, 60, 0);
    palScreen[0x18] = olc::Pixel(84, 90, 0);
    palScreen[0x19] = olc::Pixel(40, 114, 0);
    palScreen[0x1A] = olc::Pixel(8, 124, 0);
    palScreen[0x1B] = olc::Pixel(0, 118, 40);
    palScreen[0x1C] = olc::Pixel(0, 102, 120);
    palScreen[0x1D] = olc::Pixel(0, 0, 0);
    palScreen[0x1E] = olc::Pixel(0, 0, 0);
    palScreen[0x1F] = olc::Pixel(0, 0, 0);

    palScreen[0x20] = olc::Pixel(236, 238, 236);
    palScreen[0x21] = olc::Pixel(76, 154, 236);
    palScreen[0x22] = olc::Pixel(120, 124, 236);
    palScreen[0x23] = olc::Pixel(176, 98, 236);
    palScreen[0x24] = olc::Pixel(228, 84, 236);
    palScreen[0x25] = olc::Pixel(236, 88, 180);
    palScreen[0x26] = olc::Pixel(236, 106, 100);
    palScreen[0x27] = olc::Pixel(212, 136, 32);
    palScreen[0x28] = olc::Pixel(160, 170, 0);
    palScreen[0x29] = olc::Pixel(116, 196, 0);
    palScreen[0x2A] = olc::Pixel(76, 208, 32);
    palScreen[0x2B] = olc::Pixel(56, 204, 108);
    palScreen[0x2C] = olc::Pixel(56, 180, 204);
    palScreen[0x2D] = olc::Pixel(60, 60, 60);
    palScreen[0x2E] = olc::Pixel(0, 0, 0);
    palScreen[0x2F] = olc::Pixel(0, 0, 0);

    palScreen[0x30] = olc::Pixel(236, 238, 236);
    palScreen[0x31] = olc::Pixel(168, 204, 236);
    palScreen[0x32] = olc::Pixel(188, 188, 236);
    palScreen[0x33] = olc::Pixel(212, 178, 236);
    palScreen[0x34] = olc::Pixel(236, 174, 236);
    palScreen[0x35] = olc::Pixel(236, 174, 212);
    palScreen[0x36] = olc::Pixel(236, 180, 176);
    palScreen[0x37] = olc::Pixel(228, 196, 144);
    palScreen[0x38] = olc::Pixel(204, 210, 120);
    palScreen[0x39] = olc::Pixel(180, 222, 120);
    palScreen[0x3A] = olc::Pixel(168, 226, 144);
    palScreen[0x3B] = olc::Pixel(152, 226, 180);
    palScreen[0x3C] = olc::Pixel(160, 214, 228);
    palScreen[0x3D] = olc::Pixel(160, 162, 160);
    palScreen[0x3E] = olc::Pixel(0, 0, 0);
    palScreen[0x3F] = olc::Pixel(0, 0, 0);
}

pineapple2C02::~pineapple2C02() {}

void pineapple2C02::ppuWrite(uint16_t addr, uint8_t data) {
    addr &= 0x3FFF;
    if (cart->ppuWrite(addr, data)) {} // Same principle as in the Main Bus.
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        // In general, this is a ROM, but in some cartridges, this is a RAM
        tblPattern[(addr & 0x1000) >> 12][addr & 0x0FF] = data;
    }
    else if (addr >= 0x2000 && addr <= 0x3EFF) {
        addr &= 0x0FFF;
        if (cart->mirror == cartridge::MIRROR::VERTICAL)
        {
            // Following code taken from OLCNES emulator by OneLoneCoder
            if (addr >= 0x0000 && addr <= 0x03FF)
                tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x0400 && addr <= 0x07FF)
                tblName[1][addr & 0x03FF] = data;
            if (addr >= 0x0800 && addr <= 0x0BFF)
                tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                tblName[1][addr & 0x03FF] = data;
        }
        else if (cart->mirror == cartridge::MIRROR::HORIZONTAL)
        {
            // Horizontal
            if (addr >= 0x0000 && addr <= 0x03FF)
                tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x0400 && addr <= 0x07FF)
                tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x0800 && addr <= 0x0BFF)
                tblName[1][addr & 0x03FF] = data;
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                tblName[1][addr & 0x03FF] = data;
        }

    }
    else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        addr &= 0x001F;
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;
        tblPalette[addr] = data;
    }
}

uint8_t pineapple2C02::ppuRead(uint16_t addr, bool rdonly) {
    uint8_t data = 0x00;
    addr &= 0x3FFF;

    if (cart->ppuRead(addr, data)) {} // Same principle as in the Main Bus.
    else if (addr >= 0x0000 && addr <= 0x1FFF) { // Access the pattern table
        data = tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
        // First part of the array chooses the first or second chunk of the table (first 4 KB or next 4 KB)
        // Second part chooses the offset by masking the remaining bits
        // This is the ROM, so a write function is unnecessary
    }
    else if (addr >= 0x2000 && addr <= 0x3EFF) {
        addr&=0x0FFF;
        if (cart->mirror == cartridge::MIRROR::VERTICAL) {
            // vertical mirroring
            // hardcoded
            if (addr >= 0x0000 && addr <= 0x03FF)
                data = tblName[0][addr & 0x03FF];
            if (addr >= 0x0400 && addr <= 0x07FF)
                data = tblName[1][addr & 0x03FF];
            if (addr >= 0x0800 && addr <= 0x0BFF)
                data = tblName[0][addr & 0x03FF];
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                data = tblName[1][addr & 0x03FF];
        }
        else if (cart->mirror == cartridge::MIRROR::HORIZONTAL) {
            // horizontal
            if (addr >= 0x0000 && addr <= 0x03FF)
                data = tblName[0][addr & 0x03FF];
            if (addr >= 0x0400 && addr <= 0x07FF)
                data = tblName[0][addr & 0x03FF];
            if (addr >= 0x0800 && addr <= 0x0BFF)
                data = tblName[1][addr & 0x03FF];
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                data = tblName[1][addr & 0x03FF];
        }
    }
    else if (addr >= 0x3F00 && addr <= 0x3FFF) { // Table palette array
        // Select the appropriate index by masking the last 5 bits
        addr &= 0x001F;
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;
        data = tblPalette[addr];
        // The first three are colours, the last is the transparent bit
        // WE HAVE HARDCODED THE MIRRORING
    }
    return data;
}

olc::Sprite& pineapple2C02::GetPatternTable(uint8_t i, uint8_t palette) { // The Pattern table contains the spritesheet
    // for later use
    for (uint16_t nTileY = 0; nTileY < 16; nTileY++) { // It is split into two chunks of 4 KB each
        for (uint16_t nTileX = 0; nTileX < 16; nTileX++) { // Each of them are split into 16x16 square grids of 8x8 pixels
            uint16_t nOffset = nTileY * 256 + nTileX * 16; // In HEX, this is essentially H*100 + D*10
            // Each of these 8x8 pixels need to be populated with colours
            for (uint16_t row = 0; row < 8; row++) {
                // there are 2 bits each to describe each colour, divided into
                // the most significant bit MSB and the least significant bit, LSB
                uint8_t tile_lsb = ppuRead(i * 0x1000 + nOffset + row + 0);
                uint8_t tile_msb = ppuRead(i * 0x1000 + nOffset + row + 8);
                // This contains data for the entire 8x8 square
                for (uint16_t col = 0; col < 8; col++) {
                    uint8_t pixel = (tile_lsb & 0x01) + (tile_msb & 0x01);
                    tile_lsb >>= 1;
                    tile_msb >>= 1;

                    sprPatternTable[i].SetPixel(
                        nTileX * 8 + (7 - col), nTileY + row, // (we're starting from the top-left, so math works out)
                        GetColourFromPaletteRam(palette, pixel)
                    );
                }
            }
        }
    }
    return sprPatternTable[i];
}
olc::Sprite& pineapple2C02::GetScreen()
{
    // Simply returns the current sprite holding the rendered screen
    return sprScreen;
}

olc::Pixel& pineapple2C02::GetColourFromPaletteRam(uint8_t palette, uint8_t pixel) {
    // we need to have the PPU access the memory at $3F00 + the offset for the palette
    return palScreen[ppuRead(0x3F00 + (palette << 2) + pixel)  & 0x3F];
}

uint8_t pineapple2C02::cpuRead(uint16_t addr, bool rdonly) {
    uint8_t data = 0x00;
    switch (addr) {
    case 0x0000:
        break;
    case 0x0001:
        break;
    case 0x0002: // Status
        status.vertical_blank = 1;
        data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F); // not used in games
        status.vertical_blank = 0;
        address_latch = 0x00;
        break;
    case 0x0003:
        break;
    case 0x0004:
        break;
    case 0x0005:
        break;
    case 0x0006:
        break;
    case 0x0007: // PPU Data
        data = ppu_data_buffer; // reading is delayed.
        // so, data is only read one cycle late.
        // first cycle -> data gets 0x00, buffer gets actual info 1
        // next cycle -> data gets actual info 1, buffers gets actual info 2, and so on.
        ppu_data_buffer = ppuRead(ppu_address);

        // This delay is true for all ppu address range except the palette.
        if (ppu_address > 0x3f00) data = ppu_data_buffer;
        break;

    }
    return data;
}

void pineapple2C02::cpuWrite(uint16_t addr, uint8_t data) {
    switch (addr) {
    case 0x0000: // Control
        control.reg = data; // uses the control union struct
        break;
    case 0x0001: // Mask
        mask.reg = data;
        break;
    case 0x0002: // Status
        break;
    case 0x0003: // OAM Address
        break;
    case 0x0004: // OAM Data
        break;
    case 0x0005: // Scroll
        break;
    case 0x0006: // PPU Address
        if (address_latch == 0) {
            ppu_address = (ppu_address & 0x00FF) | (data<<8); // store the other 8 bits
            // ppu_address & 0x00FF clears up the top 8 bits to get OR'ed with data
            address_latch = 1; // primes for the next part of the 16-bit address
        }
        if (address_latch == 1) {
            ppu_address = (ppu_address & 0xFF00) | (data); // store the 8 bits of the ppu address
            // ppu_address  & 0xFF00 clears up the last 8 bits to get OR'ed with data
            address_latch = 0; // primes the thing to load the the other part of the 16-bit address
        }
        break;
    case 0x0007: //
        ppuWrite(ppu_address, data);
        ppu_address += (control.increment_mode ? 32 : 1);
        break;

    }
}
void pineapple2C02::ConnectCartridge(const std::shared_ptr<cartridge>& cartridge) {
    this->cart = cartridge;
} // connect the LOCAL cartridge variable to the cartridge pointer.

void pineapple2C02::reset()
{
	address_latch = 0x00;
	ppu_data_buffer = 0x00;
	scanline = 0;
	cycle = 0;
	status.reg = 0x00;
	mask.reg = 0x00;
	control.reg = 0x00;
}


void pineapple2C02::clock() {
    // if (scanline == -1 && cycle == 1) {
    //     status.vertical_blank = 0;
    // }

    // if (scanline == 241 && cycle == 1) {
    //     status.vertical_blank = 1;
    //     // The vertical blank period is a period of time when the part of the screen below the 240 vertical scanlines
    //     // are being accessed. In this time, the CPU can configure and talk to the PPU without adversely affecting
    //     // what's on screen.
    //     if (control.enable_nmi)
    //         nmi = true; // Non maskable interrupt is set to true.
    //         // Now, bus.cpp must check that the interrupt has been triggered
    // }
    sprScreen.SetPixel(cycle - 1, scanline, palScreen[(rand() % 2) ? 0x3F : 0x30]);
    cycle++;

    // height and width render
    // scanlines are horizontal lines here
    // original NES had resolution of 256 x 240
    // hardcoded wrt resolution
    if (cycle >= 341) {
        cycle = 0;
        scanline++;
        if (scanline >= 261) {
            scanline = -1;
            frame_complete = true;
        }
    }

}