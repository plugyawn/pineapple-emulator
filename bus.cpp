#include "bus.h"
#pragma once

bus::bus() {
    // Clear RAM and set to 0
    // iterates through standard array
    for (auto& iter : cpuRam) iter = 0x00;

    // connect CPU to the comms bus
    cpu.ConnectBus(this);
}


bus::~bus() = default;

void bus::cpuWrite(uint16_t addr, uint8_t data) {
    // BRIEF OVERVIEW -->
    //THE BUS function is essentially an IF-ELSE-IF construct that asks
    //�Is this address in your range? No, okay is this in YOUR range? No, is this in <this other person>�s range� and so on.
    //We give cartridge first dibs, and move on to RAM, PPU, so on.

    if (cart->cpuWrite(addr, data)) {}  // The cartridge gets to decide if the instruction is meant for IT
    else if (addr >= 0x0000 && addr <= 0x1FFF) { // Check if address is meant for the CPU
        cpuRam[addr & 0x07FF] = data; // like modulus, check mirroring.
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF) { // Check if address is meant for the PPU
        ppu.cpuWrite(addr & 0x0007, data);
    }
}

uint8_t bus::cpuRead(uint16_t addr, bool bReadOnly) {
    uint8_t data = 0x00;
    if (cart->cpuRead(addr, data))
	{
		// Cartridge Address Range
	}
    else if (addr > 0x0000 && addr <= 0x1FFF) {
        data = cpuRam[addr & 0x07FF]; // like modulus
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF) {
        data = ppu.cpuRead(addr & 0x0007, bReadOnly);
    }
    return data; // take address and return what is at that position in RAM array
    // it returns 0 if overflow/underflow
}

void bus::reset() {
    cart->reset();
    ppu.reset();
    cpu.reset();
    nSystemClockCounter = 0;
} // This is what happens when the NES is reset. The CPU is reset, and the clock counter goes to 0.

void bus::insertCartridge(const std::shared_ptr<cartridge> &cartridge) {
    this->cart = cartridge; // sets the connected cartridge to the cartridge
    ppu.ConnectCartridge(cartridge); // connects cartridge to the picture processing unit.
} // insert Cartridge into the bus. How does this work? We primarily need to connect the cartridge to the PPU.

void bus::clock() {
    ppu.clock();

    // Remember that the CPU clock runs 3 times slower than the ppu

    if (nSystemClockCounter % 3 == 0) {
        cpu.clock();
    }

    if (ppu.nmi) { // check ppu nmi, after scanline hits 241 and enters vertical blanking
        ppu.nmi = false;
        cpu.nmi();
    }

    nSystemClockCounter++;
}
