#include "SPUInterpreter.hpp"
#include <PlayStation3.hpp>


#define UNIMPL_INSTR(name)                                                      \
void SPUInterpreter::name(const SPUInstruction& instr) {                        \
    Helpers::panic("Unimplemented instruction %s @ 0x%08x\n", #name, state.pc); \
}

SPUInterpreter::SPUInterpreter(PlayStation3* ps3) : SPU(ps3) {
    printf("Initializing instruction table\n");
    // Initialize instruction table
    for (int i = 0; i < INSTR_TABLE_SIZE; i++)
        instr_table[i] = &SPUInterpreter::unimpl;

    registerInstruction(10, 0x0,    &SPUInterpreter::stop);
    registerInstruction(10, 0x1,    &SPUInterpreter::lnop);
    registerInstruction(10, 0x2,    &SPUInterpreter::sync);
    registerInstruction(10, 0x3,    &SPUInterpreter::dsync);
    registerInstruction(10, 0xc,    &SPUInterpreter::mfspr);
    registerInstruction(10, 0xd,    &SPUInterpreter::rdch);
    registerInstruction(10, 0xf,    &SPUInterpreter::rchcnt);
    registerInstruction(10, 0x40,   &SPUInterpreter::sf);
    registerInstruction(10, 0x41,   &SPUInterpreter::or_);
    registerInstruction(10, 0x42,   &SPUInterpreter::bg);
    registerInstruction(10, 0x48,   &SPUInterpreter::sfh);
    registerInstruction(10, 0x49,   &SPUInterpreter::nor);
    registerInstruction(10, 0x53,   &SPUInterpreter::absdb);
    registerInstruction(10, 0x58,   &SPUInterpreter::rot);
    registerInstruction(10, 0x59,   &SPUInterpreter::rotm);
    registerInstruction(10, 0x5a,   &SPUInterpreter::rotma);
    registerInstruction(10, 0x5b,   &SPUInterpreter::shl);
    registerInstruction(10, 0x5c,   &SPUInterpreter::roth);
    registerInstruction(10, 0x5d,   &SPUInterpreter::rothm);
    registerInstruction(10, 0x5e,   &SPUInterpreter::rotmah);
    registerInstruction(10, 0x5f,   &SPUInterpreter::shlh);
    registerInstruction(10, 0x78,   &SPUInterpreter::roti);
    registerInstruction(10, 0x79,   &SPUInterpreter::rotmi);
    registerInstruction(10, 0x7a,   &SPUInterpreter::rotmai);
    registerInstruction(10, 0x7b,   &SPUInterpreter::shli);
    registerInstruction(10, 0x7c,   &SPUInterpreter::rothi);
    registerInstruction(10, 0x7d,   &SPUInterpreter::rothmi);
    registerInstruction(10, 0x7e,   &SPUInterpreter::rotmahi);
    registerInstruction(10, 0x7f,   &SPUInterpreter::shlhi);
    registerInstruction(10, 0xc0,   &SPUInterpreter::a);
    registerInstruction(10, 0xc1,   &SPUInterpreter::and_);
    registerInstruction(10, 0xc2,   &SPUInterpreter::cg);
    registerInstruction(10, 0xc8,   &SPUInterpreter::ah);
    registerInstruction(10, 0xc9,   &SPUInterpreter::nand);
    registerInstruction(10, 0xd3,   &SPUInterpreter::avgb);
    registerInstruction(10, 0x10c,  &SPUInterpreter::mtspr);
    registerInstruction(10, 0x10d,  &SPUInterpreter::wrch);
    registerInstruction(10, 0x128,  &SPUInterpreter::biz);
    registerInstruction(10, 0x129,  &SPUInterpreter::binz);
    registerInstruction(10, 0x12a,  &SPUInterpreter::bihz);
    registerInstruction(10, 0x12b,  &SPUInterpreter::bihnz);
    registerInstruction(10, 0x140,  &SPUInterpreter::stopd);
    registerInstruction(10, 0x144,  &SPUInterpreter::stqx);
    registerInstruction(10, 0x1a8,  &SPUInterpreter::bi);
    registerInstruction(10, 0x1a9,  &SPUInterpreter::bisl);
    registerInstruction(10, 0x1aa,  &SPUInterpreter::iret);
    registerInstruction(10, 0x1ab,  &SPUInterpreter::bisled);
    registerInstruction(10, 0x1ac,  &SPUInterpreter::hbr);
    registerInstruction(10, 0x1b0,  &SPUInterpreter::gb);
    registerInstruction(10, 0x1b1,  &SPUInterpreter::gbh);
    registerInstruction(10, 0x1b2,  &SPUInterpreter::gbb);
    registerInstruction(10, 0x1b4,  &SPUInterpreter::fsm);
    registerInstruction(10, 0x1b5,  &SPUInterpreter::fsmh);
    registerInstruction(10, 0x1b6,  &SPUInterpreter::fsmb);
    registerInstruction(10, 0x1b8,  &SPUInterpreter::frest);
    registerInstruction(10, 0x1b9,  &SPUInterpreter::frsqest);
    registerInstruction(10, 0x1c4,  &SPUInterpreter::lqx);
    registerInstruction(10, 0x1cc,  &SPUInterpreter::rotqbybi);
    registerInstruction(10, 0x1cd,  &SPUInterpreter::rotqmbybi);
    registerInstruction(10, 0x1cf,  &SPUInterpreter::shlqbybi);
    registerInstruction(10, 0x1d4,  &SPUInterpreter::cbx);
    registerInstruction(10, 0x1d5,  &SPUInterpreter::chx);
    registerInstruction(10, 0x1d6,  &SPUInterpreter::cwx);
    registerInstruction(10, 0x1d7,  &SPUInterpreter::cdx);
    registerInstruction(10, 0x1d8,  &SPUInterpreter::rotqbi);
    registerInstruction(10, 0x1d9,  &SPUInterpreter::rotqmbi);
    registerInstruction(10, 0x1db,  &SPUInterpreter::shlqbi);
    registerInstruction(10, 0x1dc,  &SPUInterpreter::rotqby);
    registerInstruction(10, 0x1dd,  &SPUInterpreter::rotqmby);
    registerInstruction(10, 0x1df,  &SPUInterpreter::shlqby);
    registerInstruction(10, 0x1f0,  &SPUInterpreter::orx);
    registerInstruction(10, 0x1f4,  &SPUInterpreter::cbd);
    registerInstruction(10, 0x1f5,  &SPUInterpreter::chd);
    registerInstruction(10, 0x1f6,  &SPUInterpreter::cwd);
    registerInstruction(10, 0x1f7,  &SPUInterpreter::cdd);
    registerInstruction(10, 0x1f8,  &SPUInterpreter::rotqbii);
    registerInstruction(10, 0x1f9,  &SPUInterpreter::rotqmbii);
    registerInstruction(10, 0x1fb,  &SPUInterpreter::shlqbii);
    registerInstruction(10, 0x1fc,  &SPUInterpreter::rotqbyi);
    registerInstruction(10, 0x1fd,  &SPUInterpreter::rotqmbyi);
    registerInstruction(10, 0x1ff,  &SPUInterpreter::shlqbyi);
    registerInstruction(10, 0x201,  &SPUInterpreter::nop);
    registerInstruction(10, 0x240,  &SPUInterpreter::cgt);
    registerInstruction(10, 0x241,  &SPUInterpreter::xor_);
    registerInstruction(10, 0x248,  &SPUInterpreter::cgth);
    registerInstruction(10, 0x249,  &SPUInterpreter::eqv);
    registerInstruction(10, 0x250,  &SPUInterpreter::cgtb);
    registerInstruction(10, 0x253,  &SPUInterpreter::sumb);
    registerInstruction(10, 0x258,  &SPUInterpreter::hgt);
    registerInstruction(10, 0x2a5,  &SPUInterpreter::clz);
    registerInstruction(10, 0x2a6,  &SPUInterpreter::xswd);
    registerInstruction(10, 0x2ae,  &SPUInterpreter::xshw);
    registerInstruction(10, 0x2b4,  &SPUInterpreter::cntb);
    registerInstruction(10, 0x2b6,  &SPUInterpreter::xsbh);
    registerInstruction(10, 0x2c0,  &SPUInterpreter::clgt);
    registerInstruction(10, 0x2c1,  &SPUInterpreter::andc);
    registerInstruction(10, 0x2c2,  &SPUInterpreter::fcgt);
    registerInstruction(10, 0x2c3,  &SPUInterpreter::dfcgt);
    registerInstruction(10, 0x2c4,  &SPUInterpreter::fa);
    registerInstruction(10, 0x2c5,  &SPUInterpreter::fs);
    registerInstruction(10, 0x2c6,  &SPUInterpreter::fm);
    registerInstruction(10, 0x2c8,  &SPUInterpreter::clgth);
    registerInstruction(10, 0x2c9,  &SPUInterpreter::orc);
    registerInstruction(10, 0x2ca,  &SPUInterpreter::fcmgt);
    registerInstruction(10, 0x2cb,  &SPUInterpreter::dfcmgt);
    registerInstruction(10, 0x2cc,  &SPUInterpreter::dfa);
    registerInstruction(10, 0x2cd,  &SPUInterpreter::dfs);
    registerInstruction(10, 0x2ce,  &SPUInterpreter::dfm);
    registerInstruction(10, 0x2d0,  &SPUInterpreter::clgtb);
    registerInstruction(10, 0x2d8,  &SPUInterpreter::hlgt);
    registerInstruction(10, 0x35c,  &SPUInterpreter::dfma);
    registerInstruction(10, 0x35d,  &SPUInterpreter::dfms);
    registerInstruction(10, 0x35e,  &SPUInterpreter::dfnms);
    registerInstruction(10, 0x35f,  &SPUInterpreter::dfnma);
    registerInstruction(10, 0x3c0,  &SPUInterpreter::ceq);
    registerInstruction(10, 0x3ce,  &SPUInterpreter::mpyhhu);
    registerInstruction(10, 0x340,  &SPUInterpreter::addx);
    registerInstruction(10, 0x341,  &SPUInterpreter::sfx);
    registerInstruction(10, 0x342,  &SPUInterpreter::cgx);
    registerInstruction(10, 0x343,  &SPUInterpreter::bgx);
    registerInstruction(10, 0x346,  &SPUInterpreter::mpyhha);
    registerInstruction(10, 0x34e,  &SPUInterpreter::mpyhhau);
    registerInstruction(10, 0x398,  &SPUInterpreter::fscrrd);
    registerInstruction(10, 0x3b8,  &SPUInterpreter::fesd);
    registerInstruction(10, 0x3b9,  &SPUInterpreter::frds);
    registerInstruction(10, 0x3ba,  &SPUInterpreter::fscrwr);
    registerInstruction(10, 0x3bf,  &SPUInterpreter::dftsv);
    registerInstruction(10, 0x3c2,  &SPUInterpreter::fceq);
    registerInstruction(10, 0x3c3,  &SPUInterpreter::dfceq);
    registerInstruction(10, 0x3c4,  &SPUInterpreter::mpy);
    registerInstruction(10, 0x3c5,  &SPUInterpreter::mpyh);
    registerInstruction(10, 0x3c6,  &SPUInterpreter::mpyhh);
    registerInstruction(10, 0x3c7,  &SPUInterpreter::mpys);
    registerInstruction(10, 0x3c8,  &SPUInterpreter::ceqh);
    registerInstruction(10, 0x3ca,  &SPUInterpreter::fcmeq);
    registerInstruction(10, 0x3cb,  &SPUInterpreter::dfcmeq);
    registerInstruction(10, 0x3cc,  &SPUInterpreter::mpyu);
    registerInstruction(10, 0x3d0,  &SPUInterpreter::ceqb);
    registerInstruction(10, 0x3d4,  &SPUInterpreter::fi);
    registerInstruction(10, 0x3d8,  &SPUInterpreter::heq);
    registerInstruction(9, 0x1d8,   &SPUInterpreter::cflts);
    registerInstruction(9, 0x1d9,   &SPUInterpreter::cfltu);
    registerInstruction(9, 0x1da,   &SPUInterpreter::csflt);
    registerInstruction(9, 0x1db,   &SPUInterpreter::cuflt);
    registerInstruction(8, 0x40,    &SPUInterpreter::brz);
    registerInstruction(8, 0x41,    &SPUInterpreter::stqa);
    registerInstruction(8, 0x42,    &SPUInterpreter::brnz);
    registerInstruction(8, 0x44,    &SPUInterpreter::brhz);
    registerInstruction(8, 0x46,    &SPUInterpreter::brhnz);
    registerInstruction(8, 0x47,    &SPUInterpreter::stqr);
    registerInstruction(8, 0x60,    &SPUInterpreter::bra);
    registerInstruction(8, 0x61,    &SPUInterpreter::lqa);
    registerInstruction(8, 0x62,    &SPUInterpreter::brasl);
    registerInstruction(8, 0x64,    &SPUInterpreter::br);
    registerInstruction(8, 0x65,    &SPUInterpreter::fsmbi);
    registerInstruction(8, 0x66,    &SPUInterpreter::brsl);
    registerInstruction(8, 0x67,    &SPUInterpreter::lqr);
    registerInstruction(8, 0x81,    &SPUInterpreter::il);
    registerInstruction(8, 0x82,    &SPUInterpreter::ilhu);
    registerInstruction(8, 0x83,    &SPUInterpreter::ilh);
    registerInstruction(8, 0xc1,    &SPUInterpreter::iohl);
    registerInstruction(7, 0x4,     &SPUInterpreter::ori);
    registerInstruction(7, 0x5,     &SPUInterpreter::orhi);
    registerInstruction(7, 0x6,     &SPUInterpreter::orbi);
    registerInstruction(7, 0xc,     &SPUInterpreter::sfi);
    registerInstruction(7, 0xd,     &SPUInterpreter::sfhi);
    registerInstruction(7, 0x14,    &SPUInterpreter::andi);
    registerInstruction(7, 0x15,    &SPUInterpreter::andhi);
    registerInstruction(7, 0x16,    &SPUInterpreter::andbi);
    registerInstruction(7, 0x1c,    &SPUInterpreter::ai);
    registerInstruction(7, 0x1d,    &SPUInterpreter::ahi);
    registerInstruction(7, 0x24,    &SPUInterpreter::stqd);
    registerInstruction(7, 0x34,    &SPUInterpreter::lqd);
    registerInstruction(7, 0x44,    &SPUInterpreter::xori);
    registerInstruction(7, 0x45,    &SPUInterpreter::xorhi);
    registerInstruction(7, 0x46,    &SPUInterpreter::xorbi);
    registerInstruction(7, 0x4c,    &SPUInterpreter::cgti);
    registerInstruction(7, 0x4d,    &SPUInterpreter::cgthi);
    registerInstruction(7, 0x4e,    &SPUInterpreter::cgtbi);
    registerInstruction(7, 0x4f,    &SPUInterpreter::hgti);
    registerInstruction(7, 0x5c,    &SPUInterpreter::clgti);
    registerInstruction(7, 0x5d,    &SPUInterpreter::clgthi);
    registerInstruction(7, 0x5e,    &SPUInterpreter::clgtbi);
    registerInstruction(7, 0x5f,    &SPUInterpreter::hlgti);
    registerInstruction(7, 0x74,    &SPUInterpreter::mpyi);
    registerInstruction(7, 0x75,    &SPUInterpreter::mpyui);
    registerInstruction(7, 0x7c,    &SPUInterpreter::ceqi);
    registerInstruction(7, 0x7d,    &SPUInterpreter::ceqhi);
    registerInstruction(7, 0x7e,    &SPUInterpreter::ceqbi);
    registerInstruction(7, 0x7f,    &SPUInterpreter::heqi);
    registerInstruction(6, 0x8,     &SPUInterpreter::hbra);
    registerInstruction(6, 0x9,     &SPUInterpreter::hbrr);
    registerInstruction(6, 0x21,    &SPUInterpreter::ila);
    registerInstruction(3, 0x8,     &SPUInterpreter::selb);
    registerInstruction(3, 0xb,     &SPUInterpreter::shufb);
    registerInstruction(3, 0xc,     &SPUInterpreter::mpya);
    registerInstruction(3, 0xd,     &SPUInterpreter::fnms);
    registerInstruction(3, 0xe,     &SPUInterpreter::fma);
    registerInstruction(3, 0xf,     &SPUInterpreter::fms);
}

void SPUInterpreter::step() {
    if (!enabled) return; 

    const SPUInstruction instr = { .raw = read<u32>(state.pc) };
    const auto opc = (instr.raw >> (31 - INSTR_BITS)) & INSTR_MASK;

    (*this.*instr_table[opc])(instr);
    printState();
    state.pc += 4;
}

void SPUInterpreter::registerInstruction(u32 size, u32 opc, void (SPUInterpreter::*handler)(const SPUInstruction&)) {
    for (int i = 0; i < (1 << (INSTR_BITS - size)); i++) {
        instr_table[(opc << (INSTR_BITS - size)) | i] = handler;
    }
}

// Instructions

void SPUInterpreter::unimpl(const SPUInstruction& instr) {
    Helpers::panic("Unimplemented instruction\n");
}

void SPUInterpreter::cwd(const SPUInstruction& instr) {
    const u32 t = (state.gprs[instr.ra].w[3] + ext<s8, 7>(instr.i7)) & 0xc;
    state.gprs[instr.rt].dw[1] = 0x1011121314151617;
    state.gprs[instr.rt].dw[0] = 0x18191A1B1C1D1E1F;
    state.gprs[instr.rt].w[3 - (t >> 2)] = 0x00010203;
}

void SPUInterpreter::cdd(const SPUInstruction& instr) {
    const u32 t = (state.gprs[instr.ra].w[3] + ext<s8, 7>(instr.i7)) & 0x8;
    state.gprs[instr.rt].dw[1] = 0x1011121314151617;
    state.gprs[instr.rt].dw[0] = 0x18191A1B1C1D1E1F;
    state.gprs[instr.rt].dw[1 - (t >> 3)] = 0x0001020304050607;
}

void SPUInterpreter::lqa(const SPUInstruction& instr) {
    const u32 addr = (instr.i16 << 2) & 0x3fff0;
    for (int i = 0; i < 16; i++)
        state.gprs[instr.rt].b[15 - i] = ls[addr + i];
}

void SPUInterpreter::lqr(const SPUInstruction& instr) {
    const u32 addr = (state.pc + (instr.i16 << 2)) & 0x3fff0;
    for (int i = 0; i < 16; i++)
        state.gprs[instr.rt].b[15 - i] = ls[addr + i];
}

void SPUInterpreter::hbrr(const SPUInstruction& instr) {}

void SPUInterpreter::ila(const SPUInstruction& instr) {
    state.gprs[instr.rt].w[0] = instr.i18;
    state.gprs[instr.rt].w[1] = instr.i18;
    state.gprs[instr.rt].w[2] = instr.i18;
    state.gprs[instr.rt].w[3] = instr.i18;
}

void SPUInterpreter::shufb(const SPUInstruction& instr) {
    u8 src[32];
    for (int i = 0; i < 16; i++) src[i +  0] = state.gprs[instr.rb].b[i];
    for (int i = 0; i < 16; i++) src[i + 16] = state.gprs[instr.ra].b[i];
    
    for (int i = 0; i < 16; i++) {
        const u8 b = state.gprs[instr.rc].b[i];
        if ((b >> 6) == 0b10)       state.gprs[instr.rt].b[i] = 0;
        else if ((b >> 5) == 0b110) state.gprs[instr.rt].b[i] = 0xff;
        else if ((b >> 5) == 0b111) state.gprs[instr.rt].b[i] = 0x80;
        else state.gprs[instr.rt].b[i] = src[31 - (b & 0x1f)];
    }
}

UNIMPL_INSTR(stop);
UNIMPL_INSTR(lnop);
UNIMPL_INSTR(sync);
UNIMPL_INSTR(dsync);
UNIMPL_INSTR(mfspr);
UNIMPL_INSTR(rdch);
UNIMPL_INSTR(rchcnt);
UNIMPL_INSTR(sf);
UNIMPL_INSTR(or_);
UNIMPL_INSTR(bg);
UNIMPL_INSTR(sfh);
UNIMPL_INSTR(nor);
UNIMPL_INSTR(absdb);
UNIMPL_INSTR(rot);
UNIMPL_INSTR(rotm);
UNIMPL_INSTR(rotma);
UNIMPL_INSTR(shl);
UNIMPL_INSTR(roth);
UNIMPL_INSTR(rothm);
UNIMPL_INSTR(rotmah);
UNIMPL_INSTR(shlh);
UNIMPL_INSTR(roti);
UNIMPL_INSTR(rotmi);
UNIMPL_INSTR(rotmai);
UNIMPL_INSTR(shli);
UNIMPL_INSTR(rothi);
UNIMPL_INSTR(rothmi);
UNIMPL_INSTR(rotmahi);
UNIMPL_INSTR(shlhi);
UNIMPL_INSTR(a);
UNIMPL_INSTR(and_);
UNIMPL_INSTR(cg);
UNIMPL_INSTR(ah);
UNIMPL_INSTR(nand);
UNIMPL_INSTR(avgb);
UNIMPL_INSTR(mtspr);
UNIMPL_INSTR(wrch);
UNIMPL_INSTR(biz);
UNIMPL_INSTR(binz);
UNIMPL_INSTR(bihz);
UNIMPL_INSTR(bihnz);
UNIMPL_INSTR(stopd);
UNIMPL_INSTR(stqx);
UNIMPL_INSTR(bi);
UNIMPL_INSTR(bisl);
UNIMPL_INSTR(iret);
UNIMPL_INSTR(bisled);
UNIMPL_INSTR(hbr);
UNIMPL_INSTR(gb);
UNIMPL_INSTR(gbh);
UNIMPL_INSTR(gbb);
UNIMPL_INSTR(fsm);
UNIMPL_INSTR(fsmh);
UNIMPL_INSTR(fsmb);
UNIMPL_INSTR(frest);
UNIMPL_INSTR(frsqest);
UNIMPL_INSTR(lqx);
UNIMPL_INSTR(rotqbybi);
UNIMPL_INSTR(rotqmbybi);
UNIMPL_INSTR(shlqbybi);
UNIMPL_INSTR(cbx);
UNIMPL_INSTR(chx);
UNIMPL_INSTR(cwx);
UNIMPL_INSTR(cdx);
UNIMPL_INSTR(rotqbi);
UNIMPL_INSTR(rotqmbi);
UNIMPL_INSTR(shlqbi);
UNIMPL_INSTR(rotqby);
UNIMPL_INSTR(rotqmby);
UNIMPL_INSTR(shlqby);
UNIMPL_INSTR(orx);
UNIMPL_INSTR(cbd);
UNIMPL_INSTR(chd);
//UNIMPL_INSTR(cwd);
//UNIMPL_INSTR(cdd);
UNIMPL_INSTR(rotqbii);
UNIMPL_INSTR(rotqmbii);
UNIMPL_INSTR(shlqbii);
UNIMPL_INSTR(rotqbyi);
UNIMPL_INSTR(rotqmbyi);
UNIMPL_INSTR(shlqbyi);
UNIMPL_INSTR(nop);
UNIMPL_INSTR(cgt);
UNIMPL_INSTR(xor_);
UNIMPL_INSTR(cgth);
UNIMPL_INSTR(eqv);
UNIMPL_INSTR(cgtb);
UNIMPL_INSTR(sumb);
UNIMPL_INSTR(hgt);
UNIMPL_INSTR(clz);
UNIMPL_INSTR(xswd);
UNIMPL_INSTR(xshw);
UNIMPL_INSTR(cntb);
UNIMPL_INSTR(xsbh);
UNIMPL_INSTR(clgt);
UNIMPL_INSTR(andc);
UNIMPL_INSTR(fcgt);
UNIMPL_INSTR(dfcgt);
UNIMPL_INSTR(fa);
UNIMPL_INSTR(fs);
UNIMPL_INSTR(fm);
UNIMPL_INSTR(clgth);
UNIMPL_INSTR(orc);
UNIMPL_INSTR(fcmgt);
UNIMPL_INSTR(dfcmgt);
UNIMPL_INSTR(dfa);
UNIMPL_INSTR(dfs);
UNIMPL_INSTR(dfm);
UNIMPL_INSTR(clgtb);
UNIMPL_INSTR(hlgt);
UNIMPL_INSTR(dfma);
UNIMPL_INSTR(dfms);
UNIMPL_INSTR(dfnms);
UNIMPL_INSTR(dfnma);
UNIMPL_INSTR(ceq);
UNIMPL_INSTR(mpyhhu);
UNIMPL_INSTR(addx);
UNIMPL_INSTR(sfx);
UNIMPL_INSTR(cgx);
UNIMPL_INSTR(bgx);
UNIMPL_INSTR(mpyhha);
UNIMPL_INSTR(mpyhhau);
UNIMPL_INSTR(fscrrd);
UNIMPL_INSTR(fesd);
UNIMPL_INSTR(frds);
UNIMPL_INSTR(fscrwr);
UNIMPL_INSTR(dftsv);
UNIMPL_INSTR(fceq);
UNIMPL_INSTR(dfceq);
UNIMPL_INSTR(mpy);
UNIMPL_INSTR(mpyh);
UNIMPL_INSTR(mpyhh);
UNIMPL_INSTR(mpys);
UNIMPL_INSTR(ceqh);
UNIMPL_INSTR(fcmeq);
UNIMPL_INSTR(dfcmeq);
UNIMPL_INSTR(mpyu);
UNIMPL_INSTR(ceqb);
UNIMPL_INSTR(fi);
UNIMPL_INSTR(heq);
UNIMPL_INSTR(cflts);
UNIMPL_INSTR(cfltu);
UNIMPL_INSTR(csflt);
UNIMPL_INSTR(cuflt);
UNIMPL_INSTR(brz);
UNIMPL_INSTR(stqa);
UNIMPL_INSTR(brnz);
UNIMPL_INSTR(brhz);
UNIMPL_INSTR(brhnz);
UNIMPL_INSTR(stqr);
UNIMPL_INSTR(bra);
//UNIMPL_INSTR(lqa);
UNIMPL_INSTR(brasl);
UNIMPL_INSTR(br);
UNIMPL_INSTR(fsmbi);
UNIMPL_INSTR(brsl);
//UNIMPL_INSTR(lqr);
UNIMPL_INSTR(il);
UNIMPL_INSTR(ilhu);
UNIMPL_INSTR(ilh);
UNIMPL_INSTR(iohl);
UNIMPL_INSTR(ori);
UNIMPL_INSTR(orhi);
UNIMPL_INSTR(orbi);
UNIMPL_INSTR(sfi);
UNIMPL_INSTR(sfhi);
UNIMPL_INSTR(andi);
UNIMPL_INSTR(andhi);
UNIMPL_INSTR(andbi);
UNIMPL_INSTR(ai);
UNIMPL_INSTR(ahi);
UNIMPL_INSTR(stqd);
UNIMPL_INSTR(lqd);
UNIMPL_INSTR(xori);
UNIMPL_INSTR(xorhi);
UNIMPL_INSTR(xorbi);
UNIMPL_INSTR(cgti);
UNIMPL_INSTR(cgthi);
UNIMPL_INSTR(cgtbi);
UNIMPL_INSTR(hgti);
UNIMPL_INSTR(clgti);
UNIMPL_INSTR(clgthi);
UNIMPL_INSTR(clgtbi);
UNIMPL_INSTR(hlgti);
UNIMPL_INSTR(mpyi);
UNIMPL_INSTR(mpyui);
UNIMPL_INSTR(ceqi);
UNIMPL_INSTR(ceqhi);
UNIMPL_INSTR(ceqbi);
UNIMPL_INSTR(heqi);
UNIMPL_INSTR(hbra);
//UNIMPL_INSTR(hbrr);
//UNIMPL_INSTR(ila);
UNIMPL_INSTR(selb);
//UNIMPL_INSTR(shufb);
UNIMPL_INSTR(mpya);
UNIMPL_INSTR(fnms);
UNIMPL_INSTR(fma);
UNIMPL_INSTR(fms);