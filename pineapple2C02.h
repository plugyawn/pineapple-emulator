#include <cstdint>
#include <string>
#include <vector>
#pragma once
#include "cartridge.h"
#include "olcPixelGameEngine.h"
class pineapple2C02 {

public:
    pineapple2C02(); // Constructor
    ~pineapple2C02();

public: // private?
    uint8_t  tblName[2][1024]; // holds the name table information.
    // This is 2KB, but we split it into 2*1024 since ONE name table takes 1 KB, and the NES has 2
    uint8_t tblPalette[32]; // small RAM

    //NOT SURE
    uint8_t tblPattern[2][4096]; // NOT SURE IF THIS IS EVER USED!! CHECK!

public: // Public since everyone needs to be able to use these functions
    // Communications with main bus
    uint8_t cpuRead(uint16_t addr, bool rdonly = false);
    void cpuWrite(uint16_t addr, uint8_t data);

    // Commms with the ppu bus
    uint8_t ppuRead(uint16_t addr, bool rdonly = false);
    void ppuWrite(uint16_t addr, uint8_t data);

private:
    olc::Pixel palScreen[0x40];
    olc::Sprite sprScreen = olc::Sprite(256, 240);
    olc::Sprite sprNameTable[2] = { olc::Sprite(256,240), olc::Sprite(256,240) };
    olc::Sprite sprPatternTable[2] = { olc::Sprite(128,128), olc::Sprite(128,128) };

public:
    olc::Sprite& GetScreen();
    olc::Sprite& GetNameTable(uint8_t i);
    olc::Sprite& GetPatternTable(uint8_t i);
    olc::Pixel& GetColourFromPaletteRam(uint8_t palette, uint8_t pixel);

private:
    std::shared_ptr<cartridge> cart;

public:
    void ConnectCartridge(const std::shared_ptr<cartridge>& cartridge);
    void clock();

    bool nmi = false;

private:
    int16_t cycle = 0;
    int16_t scanline = 0;

public:
    bool frame_complete = false;
    olc::Sprite& GetPatternTable(uint8_t i, uint8_t palette);
    void reset();

private:
    // THESE ARE USED IN THE CPU READ/WRITE INSTRUCTIONS
    union { // cs.smu.ca/~porter/ has a good resource on unions that I referred to.
        // the documentation and the tutorial we followed suggested unions, so we used that.
        struct {
            uint8_t unused : 5;
            uint8_t sprite_overflow : 1;
            uint8_t sprite_zero_hit : 1;
            uint8_t vertical_blank : 1;
        };
        uint8_t reg;
    } status;

    union {
        struct {
            uint8_t grayscale : 1;
            uint8_t render_background_left : 1;
            uint8_t render_sprites_left : 1;
            uint8_t render_background : 1;
            uint8_t render_sprites : 1;
            uint8_t enhance_red : 1;
            uint8_t enhance_green : 1;
            uint8_t enhance_blue : 1;
        };
        uint8_t reg;
    } mask;

    union PPUCTRL {
        struct { // these are straight from the documentation
            uint8_t nametable_x : 1;
            uint8_t nametable_y : 1;
            uint8_t increment_mode : 1;
            uint8_t pattern_sprite : 1;
            uint8_t pattern_background : 1;
            uint8_t sprite_size : 1;
            uint8_t slave_mode : 1;
            uint8_t enable_nmi : 1; // non maskable interrupts
        };
        uint8_t reg;
    } control;

    uint8_t address_latch = 0x00; // indicates whether writing high byte or low byte
    uint8_t  ppu_data_buffer = 0x00; // when we read data from PPU, it's delayed by 1 cycle, so this is the buffer
    uint16_t  ppu_address = 0x0000; // store the compiled address

};// Destructor