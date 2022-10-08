#include <cstdint>
#include <string>
#include <vector>
#pragma once
#include <string>
#include <map>
class bus;

class pineapple6502 {

public:
    pineapple6502(); // Constructor
    ~pineapple6502(); // Destructor

public:

    void ConnectBus(bus* bus_object_arg) {
        bus = bus_object_arg;
    } // step 1 for connecting bus to the processor

    uint8_t a = 0x00; // Accumulator Register   
    uint8_t x = 0x00; // X Register
    uint8_t y = 0x00; // Y Register
    uint8_t stkp = 0x00; // Stack pointer
    uint16_t pc = 0x0000; // PC, Program Counter, accesses addresses so needs to be 16 bits
    uint8_t status = 0x00; // Status Register -- this is where the FLAGS defined below are stored

    bool complete();

    void clock(); // clock-cycles, indicates to the CPU that we want ONE clockcycle to pass
    void reset();
    void irq(); // interrupt-request
    void nmi();  // non-maskable-interrupt

    // All of them need to occur in ASYNC! But it finishes the current instruction that's processing.


    // Produces a map of strings, with keys equivalent to instruction start locations
    // in memory, for the specified address range
    std::map<uint16_t, std::string> disassemble(uint16_t nStart, uint16_t nStop);

    enum FLAGS6502 {
        // Carry Bit
        C = (1 << 0),
        // Zero
        Z = (1 << 1),
        // Disable interrupts
        I = (1 << 2),
        // Decimal Mode
        // The NES used a modified version of 6502 that didn't use the Decimal mode
        D = (1 << 3),
        // Break?
        // Tells that the break function has been invoked
        B = (1 << 4),
        // Unused status bit
        U = (1 << 5),
        // Overflow bit
        V = (1 << 6),
        // Negative
        N = (1 << 7),
        // V & N are used if the programmer uses the 6502 with signed variables
    }; // information that goes into the status register

    // Addressing Modes -- Implied, Immediate, Absolute (direct access), and more.
        // These functions will change the number of cycles to execute as the op-code which specifies the
        // Addressing mode, drastically influences how many bytes, how many cycles each duration will take.
        // So, the first byte tells what is the size and the duration.

    // Why do these have return types? Well, some OPCODES have a caveat, that says the time duration will vary
    // based on certain issues. So, depending on the instruction being executed, they might need more clock cycles
    // than they asked for.
private:


    // INTERNAL HELPER FUNCTIONS
    uint8_t fetch(); // fetches data from source, depending on the instructions
    uint8_t fetched = 0x00; // holder for fetched data
    uint16_t temp = 0x0000; // temporary variable, for convenience
    uint16_t addr_abs = 0x0000; // addressing mode dictates where to get the address from
    uint16_t addr_rel = 0x00; // BRANCH instructions can ONLY JUMP TO A RELATIVE ADDRESS
    uint8_t opcode = 0x00;
    uint8_t cycles = 0x00;
    // ADC, ABS for example takes 4 or 5 clock cycles to complete

    // The function returns a 1 or 0 depending on whether it requires it.
    // Again, this will only make a difference if BOTH addressing mode AND the opcode ask for it.
    uint8_t IMP(); // Implied
    uint8_t IMM(); // Immediate
    uint8_t ZP0();
    uint8_t ZPX();
    uint8_t ZPY();
    uint8_t REL();
    uint8_t ABS(); // Absolute
    uint8_t ABX();
    uint8_t ABY();
    uint8_t IND();
    uint8_t IZX();
    uint8_t IZY();

    // Op-Codes
        // These Op-Codes were copy-pasted from a table.
        // These are the 56 legal op-codes provided by the 6502 CPU.
        // The link to these are in the project PDF.
        // There are potentially 256 op-codes possible (2**8)

        // The documentation heavily implied that the implementation is not like a switch-case system in HLL
        // Instead, these codes turn circuitry in the CPU on and off to do their job.
        // As such, illegal op-codes affect the CPU in different ways too.

    // These op-codes here are in alphabetical order.
    // table of forward-declared functions taken from the brilliant tutorial by
        // https://github.com/OneLoneCoder/olcNES/

    // the return types are to identify 0 or 1, saying if they have variable clock cycles or not.

    uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
    uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
    uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
    uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
    uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
    uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
    uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
    uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
    uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
    uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
    uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
    uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
    uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
    uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();

    // What happens if we get an illegal op-code?

    uint8_t XXX();

    // We push all of them to this.

private:
    bus* bus = nullptr;
    uint8_t read(uint16_t a); // a = Address, d = Data
    void write(uint16_t a, uint8_t d);

    uint8_t GetFlag(FLAGS6502 f); // Access status register
    void SetFlag(FLAGS6502 f, bool v); // Sets the status register
    // bool V is a signal to clear or set a particular bit of the status register.
    // if true, it can set the flag to 1 if it isn't already. If it already is set to 1, no change.
    // (this depends on what we pass to it. Operation is status = status OR f.
    // if false, it sets the flag to 0 if we want to clear a 1.
    // basically, Operation is status = status AND (NOT f) (so if status = 1, f = 0, it stays 1).

    struct INSTRUCTION {
        std::string name; // the MNEMONIC (TXA, STY, SEI, etc)
        // Why? As part of the emulation, we have a disassembler. It disassembles the compiled code to human-readable.
        uint8_t(pineapple6502::* operate)() = nullptr; // OP-CODE; () implies void argument
        // Checks operation mode = OP-CODE
        uint8_t(pineapple6502::* addrmode)() = nullptr; // ADDRESSING MODE; () implies void argument
        // Checks addressing mode = defined above
        uint8_t cycles = 0x00; //number of cycles
    };

    std::vector<INSTRUCTION> lookup;

#ifdef LOGMODE
private:
    FILE* logfile = nullptr;
#endif


};


