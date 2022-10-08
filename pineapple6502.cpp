#include "pineapple6502.h"
#include "bus.h"
#pragma once
// Constructor
pineapple6502::pineapple6502() {
    using a = pineapple6502; // basically import X as Y in C++

    // WARNING!
    // This table is also from olcNES from OneLonelyCoder who wrote the great tutorial we followed.
    // This is from the A6502 data-sheet that we've attached with our PDF.
    // This populates the INSTRUCTION struct lookup vector that we initialized in our header file.

    // For our constructor,

    lookup =
        // for best effect, I recommend checking this out in an IDE like CLion or XCode. I've tried with
        // VSCode but the effect isn't as nice.

        // the first is the name ("BRK"), operate (&a::BRK, which is basically pineapple6502::BRK from the OpCodes),
        // addrmode (&a::IMM), cycles(7), and so on.

        // 16x16 table, for 256 total.
        // The ??? just means that it's unused and illegal. It goes to our XXX opcode.
    {
            { "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
            { "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
            { "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
            { "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
            { "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
            { "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
            { "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
            { "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
            { "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
            { "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
            { "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
            { "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
            { "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
            { "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
            { "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
            { "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
    };
};


// Destructor
pineapple6502::~pineapple6502() = default;

uint8_t pineapple6502::read(uint16_t a) {

    return bus->cpuRead(a, false); // bus connected to processor
    // This is absurd(ish) since read-only is set to false here.
    // This is because during normal operation, some devices on the bus might change,
    // And this is intentional.

    // DISASSEMBLER WILL WANT TO READ FROM THE DATA BUS WITHOUT MAKING ANY CHANGES TO IT.
}

void pineapple6502::clock() {
    // We'll be doing everything in one go
    // IRL, several clock cycles are required.
    // But we'll skip a few steps.
    if (cycles == 0) {
        opcode = read(pc); // returns: one 1-byte signal saying what the OPCODE is.
        pc++; // prepare it to read the next byte.

        // Get how many cycles?
        cycles = lookup[opcode].cycles;
        // the function pointer is stored, so use it to call the
        uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();
        uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

        cycles += (additional_cycle1 & additional_cycle2);
        // iff BOTH return 1, only then do we need to increase the number of cycles.
    }

    cycles--;
    // A better name would be cycles_pending, but fine.

    // In time, we're actually just doing an instruction ONCE.
}

// FUNCTIONS FOR ADDRESSING MODES

uint8_t pineapple6502::IMP() {
    fetched = a;
    return 0;
} // no additional data required for this instruction. Does something very simple (set a status bit)

uint8_t pineapple6502::IMM() {
    addr_abs = pc++;
    return 0;
}// expects the next byte of info to be the used value. So we'll prep the read addresses to point to next byte.

uint8_t pineapple6502::ZP0() {
    addr_abs = read(pc);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}// ZP0 stands for Zero Page Addressing. Allows you to access anything in the first 0xFF bytes of the address range.
// &= 0x00FF is presumably to cap the value at 0xFF = 0x00FF

// in 0xFF55, consider the upper two bits as pages, lower two as offsets.
// Consider this -- if the number is 128, it's something like 000011111111, and any number beyond that, like 111101111,
// needs to be capped down, so you can do 1111011101 & 00000111111, and get the truncated 000000(111101) as needed.

// So, here, what & 0x00FF does is, let the pages be whatever, just look at the offset.

uint8_t pineapple6502::ZPX() {
    addr_abs = (read(pc) + x);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}// ZPX is ZP0 but with the value of the x register added as an offset

uint8_t pineapple6502::ZPY() {
    addr_abs = (read(pc) + y);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}// ZPY is ZPX, but with y instead of x

uint8_t pineapple6502::REL() {
    // for branching instructions --> can only branch to around 127 memory locations around
    addr_rel = read(pc);
    pc++;
    if (addr_rel & 0x80)
        // the addr_rel is an unsigned value; so checks whether x80 (128, 0000100000) ANDs with it to produce a 0
        // that tells us whether it's positive or negative.
        addr_rel |= 0xFF00;
    // if it IS indeed negative, add 1s to the first byte of the 2 byte address.
    // COMEBACK! Why does this simplify the binary arithmetic?
    return 0;
}// Consider the pages scenario. The REL operator lets you check for bits from -127 to +128.

uint8_t pineapple6502::ABS() {
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    // left shift the hi bits by 8 bits, then add 8 bits of low.
    // 10101010 = hi, 111111 = low --> 1010101000000 OR 00000000111111 (low) = 101010101111111
    return 0;
} // absolute addressing. full 16 bits of data used.

uint8_t pineapple6502::ABX() {
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    addr_abs += x;

    if ((addr_abs & 0xFF00) != (hi << 8))
        // (hi<<8) is essentially the first 8 bits, so that's also the "page". And addr_abs & 0xFF00 also gives page.
        // Question is, does it change?
        return 1;
    else
        return 0;
} // additional clock cycle required if X register when added to ABS changes a page

uint8_t pineapple6502::ABY() {
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;

    addr_abs = (hi << 8) | lo;
    addr_abs += y;

    if ((addr_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
} // same as ABX, extra clock cycle added if page changes on adding X to the address.

// INDIRECT ADDRESSING!

// The supplied address is read in order to gather the actually intended 16-bit address.

uint8_t pineapple6502::IND() {
    uint16_t ptr_lo = read(pc);
    pc++;
    // pointer to lower 8 bits
    uint16_t ptr_hi = read(pc);
    pc++;
    // pointer to higher 8 bits

    uint16_t ptr = (ptr_hi << 8) | ptr_lo;

    // SIMULATE THE PAGE BOUNDARY BUG IN THE ORIGINAL HARDWARE
    if (ptr_lo == 0x00FF) addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
    else addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);

    // What is the bug? Basically, if the low bit is 0xFF00, then it should go to the next page boundary.
    // Instead, the device can't do that, and circles back to the same page. That causes an error.
    return 0;
} // Indirect addressing through primitive version of pointers. Has a simulated hardware bug present in OG NES hardware.

uint8_t pineapple6502::IZX() {
    uint16_t t = read(pc);
    pc++;

    uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
    uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;

    return 0;
} // actually read 8-bit address is offset by 8 bits of the X register to access mem in page 0x00 which has the 16 bits.

uint8_t pineapple6502::IZY() {
    uint16_t t = read(pc);
    pc++;

    uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
    uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;

    return 0;
} // actually read 8-bit address points to 16-bit address after offset from y register

/////////////// END OF ALL ADDRESSING MODES ///////////////////

// We now need to establish the fetch function.
// When is it used? When the addressing mode is not IMPLIED.
// The "fetched" variable is global to the CPU, so it can be accessed from anywhere.
// The function just returns for later convenience.

uint8_t pineapple6502::fetch() {
    if (lookup[opcode].addrmode != &pineapple6502::IMP) fetched = read(addr_abs);
    return fetched;
}

////////////// OP CODES FUNCTIONS //////////////

// Fetch function essential for everything but implied functions.
// That is because in implied functions it's already decided.

// Addition with carry is complicated.
// (Common competitive programming question, right?)
// Associated flags -> C (carry), V (overflow), N (negative?), Z (zero)
// The function itself --> A = A + M + C
// What does it do? Adds a value to the accumulator and adds a carry bit.
// This means you can chain together ADC commands to add more than 8 bits. That is simple.
// However, this also supports negative/positive and signed overflow.

// In unsigned, 127 + 4 = 131 = 01111111 + 00000010 = 10000100
// However, in unsigned, the first bit tells its negative, so --> it's -1 * (0000100) which is 1000100 - 100000 = 100
// Which is basically 128 + 4, wrapped around, to -4. This is OVERFLOW.

// This can be calculated (whether overflow has happened), by the following calculation:
// ~(A^M) & A^(A+M+C) (NOTE --> ^ is XOR, which gives 1 when both are DIFFERENT)
// Basically, for each bit, if A and M are of the same sign AND if both A and the RESULT are of the same sign, then
// mostly overflow hasn't happened.

// GENERALIZED
// Positive + Positive = Positive means NO OVERFLOW
// Negative + Negative = Negative means NO OVERFLOW
// Positive + Positive = Negative means YES OVERFLOW
// Negative + Negative = Negative means YES OVERFLOW
// Positive + Negative cannot overflow.

// ~(A^M) returns 1 if they're of the same sign. If they're not, it's automatically 0 --> NO OVERFLOW.
// A^(A+M+C) returns 1 if sums and the numbers are of different signs. That indicates OVERFLOW.

// Note that -128 to 127 doesn't have the CAPACITY to twice wrap around.

uint8_t pineapple6502::ADC() {
    fetch(); // puts data that we inputted into the variable 'fetched'

    // The addition itself is performed in the 16-bit domain for emulation to capture the carry bit.
    // It is stored in the 8th bit of the 16-bit carry word.
    temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);

    // the carry bit will exist in the highest bit, bit 0.
    SetFlag(C, temp > 255); // (definition reminder--> second argument is boolean for whether to set or clear)
    // Clearly, carry bit is to be set if the temp overflows beyond allowed 255)

    SetFlag(Z, (temp & 0x00FF) == 0); // if the last 8 bits are 0, then AND with 11111111 will also give 0.
    SetFlag(N, temp & 0x80); // checks for the sign bit.
    SetFlag(V, (~(uint16_t)a ^ (uint16_t)fetched & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);

    a = temp & 0x00FF;
    return 1;
} // Complicated procedure. Does a = a + m + c, where m is the argument, c is the carry bit.

uint8_t pineapple6502::SBC() {
    fetch(); // puts data in fetches

    uint16_t value = ((uint16_t)fetched) ^ 0x00FF;
    // Reminder that post inversion, the code is identical to addition.
    // 16-bit data taken for carry reasons.
    // XOR flips the bits.
    temp = (uint16_t)a + (uint16_t)value + (uint16_t)GetFlag(C);

    // the carry bit will exist in the highest bit, bit 0.
    SetFlag(C, temp > 255); // (definition reminder--> second argument is boolean for whether to set or clear)
    // Clearly, carry bit is to be set if the temp overflows beyond allowed 255)

    SetFlag(Z, (temp & 0x00FF) == 0); // if the last 8 bits are 0, then AND with 11111111 will also give 0.
    SetFlag(N, temp & 0x80); // checks for the sign bit.
    SetFlag(V, (~(uint16_t)a ^ (uint16_t)fetched & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);

    a = temp & 0x00FF;
    return 1;
} // Addition, with a negative m and a reversed ((1-c)) carry bit
// We reuse the addition machinery here.
// Express A = A - M - (1-C) as A + -1*(M) + 1 + C
// But, -M = 2's complement of M + 1
// But, +1 is already added in the equation.
// So, A = A + M' + C = A - M

uint8_t pineapple6502::AND() {
    fetch();
    a = a & fetched;
    SetFlag(Z, a == 0x00); // SetFlag 0 if a = 0
    SetFlag(N, a & 0x80); // SetFlag Negative if a has 1 at the sign-showing bit.

    return 1; // ADD 1 to N if page boundary is crossed
}// logical operation AND between the Accumulator and the fetched variable

//////// START OF BRANCH INSTRUCTIONS /////////

uint8_t pineapple6502::BCS() {
    if (GetFlag(C) == 1) {
        cycles++;
        addr_abs = pc + addr_rel;
        // the relative address we thought of earlier
        // The branch instruction automatically increases the cycle count

        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) // is page boundary crossed?
            cycles++;
        // if page boundary is crossed, increase number of cycles again

    // (ADD 1 to N if branch changes to same page, ADD 2 clock cycles if branch changes to different page)

        pc = addr_abs;
    } // Check if carry flag is set - that's the function, BRANCH if CARRY is SET
    return 0;
} // Branch if Carry flag is set

uint8_t pineapple6502::BCC() {
    // Branch if carry clear
    if (GetFlag(C) == 0) {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) cycles++;
        pc = addr_abs;
    }
    return 0;
} // Branch if carry clear, same as BCS, Branch if carry set.

uint8_t pineapple6502::BEQ() {
    if (GetFlag(Z) == 1) {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) cycles++;
        pc = addr_abs;
    }
    return 0;
} // Not sure exactly why it takes the Z flag

uint8_t pineapple6502::BMI() {
    if (GetFlag(N) == 1) {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) cycles++;
        pc = addr_abs;
    }
    return 0;
} // Branch if negative --> checks for N

uint8_t pineapple6502::BNE() {
    if ((GetFlag(Z) == 0)) {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) cycles++;
        pc = addr_abs;
    }
    return 0;
} // Branch if NOT equal. Similar to BEQ, but instead of Z = 1, it checks for Z = 0

uint8_t pineapple6502::BPL() {
    if ((GetFlag(N) == 0)) {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) cycles++;
        pc = addr_abs;
    }
    return 0;
} // Branch if positive. If N == 0, it's not negative (what if zero? No clue).

uint8_t pineapple6502::BVC() {
    if (GetFlag(V) == 0) {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) cycles++;
        pc = addr_abs;
    }
    return 0;
} // Branch if overflow clear

uint8_t pineapple6502::BVS() {
    if (GetFlag(V) == 1) {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00)) cycles++;
        pc = addr_abs;
    }
    return 0;
}

///////// END OF BRANCH INSTRUCTIONS /////////

uint8_t pineapple6502::CLC() {
    SetFlag(C, false);
    return 0;
}

// copies two bits into the V and N flags
// pretends to AND the bit with accumulator but discards result and only affects Z)
// for more info, https://retrocomputing.stackexchange.com/questions/11108/why-does-the-6502-have-the-bit-instruction
uint8_t pineapple6502::BIT() {
    fetch();
    temp = a & fetched;
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, fetched & (1 << 7));
    SetFlag(V, fetched & (1 << 6));
    return 0;
} // random function, no clue why this exists

uint8_t pineapple6502::RTI() {
    stkp++;
    status = read(0x0100 + stkp);
    // while writing, write to the Stack pointer and decrease it.
    // while reading, increase Stack pointer and read FROM it.
    status &= ~B;
    status &= ~U;
    // Resetting the status register

    stkp++;
    pc = (uint16_t)read(0x0100 + stkp); // low 8-bits of the address
    stkp++;
    pc |= (uint16_t)read(0x100 + stkp) << 8; // higher 8-bits of the code
    return 0;
} // RETURN from the Interrupt once it's actually come in.

uint8_t pineapple6502::ASL() {
    fetch();
    temp = (uint16_t)fetched << 1;
    SetFlag(C, (temp & 0xFF00) > 0);
    SetFlag(Z, (temp & 0x00FF) == 0x00);
    SetFlag(N, temp & 0x80);

    if (lookup[opcode].addrmode == &pineapple6502::IMP)
        a = temp & 0x00FF;
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
} // Arithmetic shift left function
// If Implied, then only add to accumulator
// Otherwise, write to address byte.

uint8_t pineapple6502::BRK() {
    pc++;

    SetFlag(I, 1);
    write(0x0100 + stkp, (pc >> 8) & 0x00FF);
    stkp--;
    write(0x0100 + stkp, pc & 0x00FF);
    stkp--;

    SetFlag(B, 1);
    write(0x0100 + stkp, status);
    stkp--;
    SetFlag(B, 0);

    pc = ((uint16_t)read(0xFFFE)) | ((uint16_t)read(0xFFFF) << 8);
    return 0;
}

uint8_t pineapple6502::CLD()
{
    SetFlag(D, false);
    return 0;
}

uint8_t pineapple6502::CLI() {
    SetFlag(I, false);
    return 0;
} // Clear Interrupt flag, Disable interrupts. Set I = 0

uint8_t pineapple6502::CLV() {
    SetFlag(V, false);
    return 0;
} // Clear overflow flag, V = 0

uint8_t pineapple6502::CMP() {
    fetch();
    temp = (uint16_t)a - (uint16_t)fetched;
    SetFlag(C, a >= fetched);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 1;
} // Compare accumulator. Store in C if A>=M

uint8_t pineapple6502::CPX() {
    fetch();
    temp = (uint16_t)x - (uint16_t)fetched;
    SetFlag(C, x >= fetched);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 1;
} // Compare X register

uint8_t pineapple6502::CPY() {
    fetch();
    temp = (uint16_t)y - (uint16_t)fetched;
    SetFlag(C, y >= fetched);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 1;
} // Compare Y register

uint8_t pineapple6502::DEC() {
    fetch();
    temp = fetched - 1;
    write(addr_abs, temp & 0x00FF);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
} // Decrement value at memory location. Fetch the number, decrement by 1, then re-output it at the position.

uint8_t pineapple6502::INC() {
    fetch();
    temp = fetched + 1;
    write(addr_abs, temp & 0x00FF);
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    return 0;
} // Increment value at memory location. Fetch the number, increment by 1, then re-output it at the position.

uint8_t pineapple6502::INX() {
    x++;
    SetFlag(Z, x == 0x0000);
    SetFlag(N, x & 0x0080);
    return 0;
} // Increment X register

uint8_t pineapple6502::INY() {
    y++;
    SetFlag(Z, x == 0x0000);
    SetFlag(N, x & 0x0080);
    return 0;
} // Increment Y Register

uint8_t pineapple6502::DEX() {
    x--;
    SetFlag(Z, x == 0x0000);
    SetFlag(N, x & 0x0080);
    return 0;
} // Decrement the X register

uint8_t pineapple6502::DEY() {
    y--;
    SetFlag(Z, y = 0x0000);
    SetFlag(N, y & 0x0080);
    return 0;
} // Increment the y register

uint8_t pineapple6502::EOR() {
    fetch();
    a = a ^ fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 1;
} // BITWISE XOR

uint8_t pineapple6502::JMP() {
    pc = addr_abs;
    return 0;
} // jump to address, so pc = address

uint8_t pineapple6502::JSR() {
    // push CURRENT program counter, so, pc--
    pc--;
    write(0x0100 + stkp, (pc >> 8) & 0x00FF);
    stkp--;
    write(0x0100 + stkp, pc & 0x00FF);
    stkp--;

    pc = addr_abs;
    return 0;
} // jump to a sub-routine. So, push current pc to the stack, then pc = address

uint8_t pineapple6502::LDA() {
    fetch();
    a = fetched;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x00);
    return 1;
} // Load the accumulator with fetched

uint8_t pineapple6502::LDX() {
    fetch();
    x = fetched;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x00);
    return 1;
} // Load with X register

uint8_t pineapple6502::LDY() {
    fetch();
    y = fetched;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x00);
    return 1;
} // Load with Y Register

uint8_t pineapple6502::LSR() {
    fetch();
    SetFlag(C, fetched & 0x0001);
    temp = fetched >> 1;
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);
    if (lookup[opcode].addrmode == &pineapple6502::IMP) {
        a = temp & 0x00FF;
    }
    else
        write(addr_abs, temp & 0x00FF);
    return 0;
} // Logical Shift Right operator

uint8_t pineapple6502::ORA() {
    fetch();
    a = a | fetched;
    SetFlag(Z, a == 0x0000);
    SetFlag(N, a & 0x80);
    return 1;
}

uint8_t pineapple6502::NOP() {
    // From https://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes
    switch (opcode) {
    case 0x1C:
    case 0x3C:
    case 0x5C:
    case 0x7C:
    case 0xDC:
    case 0xFC:
        return 1;
        break;
    }
    return 0;
}

// FLAG FUNCTIONS

// Returns the value of a specific bit of the status register
uint8_t pineapple6502::GetFlag(FLAGS6502 f)
{
    return ((status & f) > 0) ? 1 : 0;
}

// Sets or clears a specific bit of the status register
void pineapple6502::SetFlag(FLAGS6502 f, bool v)
{
    if (v)
        status |= f;
    else
        status &= ~f;
}



/////// STACK OPERATIONS //////

uint8_t pineapple6502::PHA() {
    // there is a base location (0x0100) allocated to which the stack pointer is an offset to.
    // When we add something to the stack, we WRITE to the stack, and then reduce the stack pointer.
    write(0x0100 + stkp, a);
    stkp--;
    return 0;
} // Push accumulator to stack

uint8_t pineapple6502::PLA() {
    stkp++;
    // Why? Because right now the stack pointer will be pointing at some new location.
    // It's primed for writing.
    a = read(0x0100 + stkp); // This address is hard-coded into the silicon
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 0;
} // Pop accumulator from stack


uint8_t pineapple6502::PHP() {
    write(0x0100 + stkp, status | B | U);
    SetFlag(B, 0);
    SetFlag(U, 0);
    stkp--;
    return 0;
}// Push status register to stack

uint8_t pineapple6502::PLP() {
    stkp++;
    status = read(0x0100 + stkp);
    SetFlag(U, 1); // Again, no idea why this is used.
    return 0;
} // Pop the status register off the stack

uint8_t pineapple6502::ROL() {
    fetch();
    temp = (uint16_t)(fetched << 1) | GetFlag(C);
    SetFlag(C, temp & 0xFF00); // shifting original bit 7 into carry
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);

    if (lookup[opcode].addrmode == &pineapple6502::IMP) {
        a = temp & 0x00FF;
    }
    else {
        write(addr_abs, temp & 0x00FF);
    }
    return 0;
} // ROL shifts all bits left one position. The Carry is shifted into bit 0 and the original bit 7 is shifted into the Carry.

uint8_t pineapple6502::ROR() {
    fetch();
    temp = (uint16_t)(GetFlag(C) << 7) | (fetched >> 1);
    SetFlag(C, temp & 0xFF00); // shifting original bit 7 into carry
    SetFlag(Z, (temp & 0x00FF) == 0x0000);
    SetFlag(N, temp & 0x0080);

    if (lookup[opcode].addrmode == &pineapple6502::IMP) {
        a = temp & 0x00FF;
    }
    else {
        write(addr_abs, temp & 0x00FF);
    }
    return 0;
} // ROR shifts all bits right one position.

uint8_t pineapple6502::RTS() {
    fetch();
    pc = (uint16_t)read(0x0100 + stkp); // lo bits
    stkp++;
    pc |= (uint16_t)read(0x0100 + stkp) << 8; // hi bits
    stkp++;

    pc++;
    return 0;
}// reverse of JSR (jump to sub-routine)

uint8_t pineapple6502::SEC() {
    SetFlag(C, true);
    return 0;
} // set carry flag, C = 1

uint8_t pineapple6502::SED() {
    SetFlag(D, true);
    return 0;
} // set decimal flag, D = 1

uint8_t pineapple6502::SEI() {
    SetFlag(I, true);
    return 0;
} // set interrupt flag to 1, I = 1

uint8_t pineapple6502::STA() {
    write(addr_abs, a);
    return 0;
} // store accumulator at address

uint8_t pineapple6502::STX() {
    write(addr_abs, x);
    return 0;
} // store X register at address

uint8_t pineapple6502::STY() {
    write(addr_abs, y);
    return 00;
} // Store Y register at address

uint8_t pineapple6502::TAX() {
    x = a;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
} // Store accumulator at X

uint8_t pineapple6502::TAY() {
    y = a;
    SetFlag(Z, y == 0x00);
    SetFlag(N, y & 0x80);
    return 0;
} // Store accumulator at Y

uint8_t pineapple6502::TSX() {
    x = stkp;
    SetFlag(Z, x == 0x00);
    SetFlag(N, x & 0x80);
    return 0;
} // transfer stack pointer to X register

uint8_t pineapple6502::TXA() {
    a = x;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 0;
} // transfer x to accumulator

uint8_t pineapple6502::TXS() {
    stkp = x;
    return 0;
} // Transfer X register to stack pointer

uint8_t pineapple6502::TYA() {
    a = y;
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);
    return 0;
} // Transfer Y register to accumulator

// CAPTURE ILLEGAL OPCODES!

uint8_t pineapple6502::XXX() {
    return 0;
}


/////////////   ////////////////



void pineapple6502::reset() {
    // configures the CPU into a known state
    a = 0;
    x = 0;
    y = 0;
    stkp = 0xFD;
    status = 0x00 | U;
    // These are all predefined constants, and absolutely arbitrary
    // PC is not set to ZERO.
    // Program data resides somewhere else

    addr_abs = 0xFFFC;
    // 0xFFFC contains two subsequent 8-bit sequences defining WHERE the PC should be
    uint16_t lo = read(addr_abs + 0);
    uint16_t hi = read(addr_abs + 1);

    pc = (hi << 8) | lo;

    addr_rel = 0x0000;
    addr_abs = 0x0000;
    fetched = 0x00;

    cycles = 8;
    // Suggested by OneLoneCoder.
} // RESET sets the CPU back to a known location.




// What interrupts do can be handled
void pineapple6502::irq() {
    // ignored if disable interrupt is not set
    // run a certain bit of code to handle interrupt
    // when an interrupt happens, the program counter and the status are stored in the stack
    // then, to HANDLE the interrupt, they jump to a new position in the address
    if (GetFlag(I) == 0) {
        write(0x0100 + stkp, (pc >> 8) & 0x00FF);
        stkp--;
        write(0x0100 + stkp, pc & 0x00FF);
        stkp--;
        // store the PC, as the PC itself is 16-bits, and each position in the stack has 8 bits

        SetFlag(B, 0); // Break status bit
        SetFlag(U, 1); // Unused status bit? Why? OLC suggested
        SetFlag(I, 1); // Disable Interrupts WHILE the interrupts are going on.
        // Interrupts cannot be interrupted in A6502
        write(0x0100 + stkp, status);
        stkp--;

        addr_abs = 0xFFFE;
        uint16_t lo = read(addr_abs + 0);
        uint16_t hi = read(addr_abs + 1);
        pc = (hi << 8) | lo;
        // This is the value of the new program counter.
        // This forces the program to jump to where the programmer has specified to handle the interrupt

        cycles = 7;
        // interrupts take a LOT of time
    }
} // Interrupt request

void pineapple6502::nmi() { // non-masking interrupt
    write(0x0100 + stkp, (pc >> 8) & 0x00FF);
    stkp--;
    write(0x0100 + stkp, (pc) & 0x00FF);
    stkp--;

    SetFlag(B, 0);
    SetFlag(U, 1);
    SetFlag(I, 1);

    write(0x0100 + stkp, status);
    stkp--;

    addr_abs = 0xFFFA; // Different from irq address, 0xFFFE
    // Looks at this specific address to find the location for the new program counter
    uint16_t  lo = read(addr_abs + 0);
    uint16_t hi = read(addr_abs + 1);
    pc = (hi << 8) | lo;

    cycles = 8;
} // This is a non-masking interrupt

void pineapple6502::write(uint16_t a, uint8_t d) {
    bus->cpuWrite(a, d); // bus connected to processor
}


///// DISASSEMBLER AND HELPER FUNCTIONS GO HERE /////////////

bool pineapple6502::complete()
{
    return cycles == 0;
}

// This is the disassembly function.
//
// CURRENTLY COPIED CODE FROM HERE ONWARDS!
// It is merely a convenience function to turn the binary instruction code into
// human readable form. Its included as part of the emulator because it can take
// advantage of many of the CPUs internal operations to do this.



std::map<uint16_t, std::string> pineapple6502::disassemble(uint16_t nStart, uint16_t nStop)
{
    uint32_t addr = nStart;
    uint8_t value = 0x00, lo = 0x00, hi = 0x00;
    std::map<uint16_t, std::string> mapLines;
    uint16_t line_addr = 0;

    // A convenient utility to convert variables into
    // hex strings because "modern C++"'s method with 
    // streams is atrocious
    auto hex = [](uint32_t n, uint8_t d)
    {
        std::string s(d, '0');
        for (int i = d - 1; i >= 0; i--, n >>= 4)
            s[i] = "0123456789ABCDEF"[n & 0xF];
        return s;
    };

    // Starting at the specified address we read an instruction
    // byte, which in turn yields information from the lookup table
    // as to how many additional bytes we need to read and what the
    // addressing mode is. I need this info to assemble human readable
    // syntax, which is different depending upon the addressing mode

    // As the instruction is decoded, a std::string is assembled
    // with the readable output
    while (addr <= (uint32_t)nStop)
    {
        line_addr = addr;

        // Prefix line with instruction address
        std::string sInst = "$" + hex(addr, 4) + ": ";

        // Read instruction, and get its readable name
        uint8_t opcode = bus->cpuRead(addr, true); addr++;
        sInst += lookup[opcode].name + " ";

        // Get oprands from desired locations, and form the
        // instruction based upon its addressing mode. These
        // routines mimmick the actual fetch routine of the
        // 6502 in order to get accurate data as part of the
        // instruction
        if (lookup[opcode].addrmode == &pineapple6502::IMP)
        {
            sInst += " {IMP}";
        }
        else if (lookup[opcode].addrmode == &pineapple6502::IMM)
        {
            value = bus->cpuRead(addr, true); addr++;
            sInst += "#$" + hex(value, 2) + " {IMM}";
        }
        else if (lookup[opcode].addrmode == &pineapple6502::ZP0)
        {
            lo = bus->cpuRead(addr, true); addr++;
            hi = 0x00;
            sInst += "$" + hex(lo, 2) + " {ZP0}";
        }
        else if (lookup[opcode].addrmode == &pineapple6502::ZPX)
        {
            lo = bus->cpuRead(addr, true); addr++;
            hi = 0x00;
            sInst += "$" + hex(lo, 2) + ", X {ZPX}";
        }
        else if (lookup[opcode].addrmode == &pineapple6502::ZPY)
        {
            lo = bus->cpuRead(addr, true); addr++;
            hi = 0x00;
            sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
        }
        else if (lookup[opcode].addrmode == &pineapple6502::IZX)
        {
            lo = bus->cpuRead(addr, true); addr++;
            hi = 0x00;
            sInst += "($" + hex(lo, 2) + ", X) {IZX}";
        }
        else if (lookup[opcode].addrmode == &pineapple6502::IZY)
        {
            lo = bus->cpuRead(addr, true); addr++;
            hi = 0x00;
            sInst += "($" + hex(lo, 2) + "), Y {IZY}";
        }
        else if (lookup[opcode].addrmode == &pineapple6502::ABS)
        {
            lo = bus->cpuRead(addr, true); addr++;
            hi = bus->cpuRead(addr, true); addr++;
            sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
        }
        else if (lookup[opcode].addrmode == &pineapple6502::ABX)
        {
            lo = bus->cpuRead(addr, true); addr++;
            hi = bus->cpuRead(addr, true); addr++;
            sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
        }
        else if (lookup[opcode].addrmode == &pineapple6502::ABY)
        {
            lo = bus->cpuRead(addr, true); addr++;
            hi = bus->cpuRead(addr, true); addr++;
            sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
        }
        else if (lookup[opcode].addrmode == &pineapple6502::IND)
        {
            lo = bus->cpuRead(addr, true); addr++;
            hi = bus->cpuRead(addr, true); addr++;
            sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
        }
        else if (lookup[opcode].addrmode == &pineapple6502::REL)
        {
            value = bus->cpuRead(addr, true); addr++;
            sInst += "$" + hex(value, 2) + " [$" + hex(addr + value, 4) + "] {REL}";
        }

        // Add the formed string to a std::map, using the instruction's
        // address as the key. This makes it convenient to look for later
        // as the instructions are variable in length, so a straight up
        // incremental index is not sufficient.
        mapLines[line_addr] = sInst;
    }

    return mapLines;
}