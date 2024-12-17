#pragma once

#include <PPU.hpp>

// Circular dependency
class PlayStation3;

using namespace PPUTypes;

class PPUInterpreter : public PPU {
public:
    PPUInterpreter(Memory& mem, PlayStation3* ps3) : PPU(mem, ps3) {}
    void step() override;

    // Main
    void subfic (const Instruction& instr);
    void cmpli  (const Instruction& instr);
    void cmpi   (const Instruction& instr);
    void addi   (const Instruction& instr);
    void addis  (const Instruction& instr);
    void bc     (const Instruction& instr);
    void sc     (const Instruction& instr);
    void b      (const Instruction& instr);
    void rlwinm (const Instruction& instr);
    void ori    (const Instruction& instr);
    void oris   (const Instruction& instr);
    void xori   (const Instruction& instr);
    void xoris  (const Instruction& instr);
    void andi   (const Instruction& instr);
    void andis  (const Instruction& instr);
    void lwz    (const Instruction& instr);
    void lbz    (const Instruction& instr);
    void lbzu   (const Instruction& instr);
    void stw    (const Instruction& instr);
    void stb    (const Instruction& instr);
    void stbu   (const Instruction& instr);
    void lhz    (const Instruction& instr);
    void sth    (const Instruction& instr);
    // G_13
    void bclr   (const Instruction& instr);
    void bcctr  (const Instruction& instr);
    // G_1E
    void rldicl (const Instruction& instr);
    void rldicr(const Instruction& instr);
    // G_1F
    void cmp    (const Instruction& instr);
    void mfcr   (const Instruction& instr);
    void cntlzw (const Instruction& instr);
    void sld    (const Instruction& instr);
    void and_   (const Instruction& instr);
    void cmpl   (const Instruction& instr);
    void subf   (const Instruction& instr);
    void andc   (const Instruction& instr);
    void neg    (const Instruction& instr);
    void mtcrf  (const Instruction& instr);
    void stdx   (const Instruction& instr);
    void addze  (const Instruction& instr);
    void mulld  (const Instruction& instr);
    void add    (const Instruction& instr);
    void xor_   (const Instruction& instr);
    void mfspr  (const Instruction& instr);
    void or_    (const Instruction& instr);
    void divdu  (const Instruction& instr);
    void mtspr  (const Instruction& instr);
    void srawi  (const Instruction& instr);
    void sradi  (const Instruction& instr);
    void extsh  (const Instruction& instr);
    void extsw  (const Instruction& instr);
    // G_3A
    void ld(const Instruction& instr);
    void ldu(const Instruction& instr);
    // G_3E
    void std    (const Instruction& instr);
    void stdu   (const Instruction& instr);
};