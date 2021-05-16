#include "opcodes6502.h"

Opcodes6502::Opcodes6502()
{
    InitMosOpCycles();
}

void Opcodes6502::InitMosOpCycles()
{
//   int implied, int immediate, int absolute, int abswp, int zp, int zpwp) {

    m_opCycles.clear();
    m_opCycles["lda"] = MOSOperandCycle("lda",0,2,4,4,3,5);
    m_opCycles["sta"] = MOSOperandCycle("sta",0,0,4,5,4,6);
    m_opCycles["sty"] = MOSOperandCycle("sty",0,0,4,0,3,4);
    m_opCycles["stx"] = MOSOperandCycle("stx",0,0,4,0,3,4);

    m_opCycles["tax"] = MOSOperandCycle("stx",2,0,0,0,0,0);
    m_opCycles["tay"] = MOSOperandCycle("tay",2,0,0,0,0,0);
    m_opCycles["tya"] = MOSOperandCycle("tya",2,0,0,0,0,0);
    m_opCycles["txa"] = MOSOperandCycle("txa",2,0,0,0,0,0);

    m_opCycles["pha"] = MOSOperandCycle("pha",3,0,0,0,0,0);
    m_opCycles["pla"] = MOSOperandCycle("pla",4,0,0,0,0,0);

    m_opCycles["dex"] = MOSOperandCycle("dex",2,0,0,0,0,0);
    m_opCycles["dey"] = MOSOperandCycle("dey",2,0,0,0,0,0);
    m_opCycles["inx"] = MOSOperandCycle("inx",2,0,0,0,0,0);
    m_opCycles["iny"] = MOSOperandCycle("iny",2,0,0,0,0,0);

    m_opCycles["and"] = MOSOperandCycle("and",0,2,4,4,3,5);
    m_opCycles["ora"] = MOSOperandCycle("ora",0,2,4,4,3,5);
    m_opCycles["eor"] = MOSOperandCycle("eor",0,2,4,4,3,5);
    m_opCycles["bit"] = MOSOperandCycle("bit",0,0,4,0,3,3);

    m_opCycles["sec"] = MOSOperandCycle("sec",2,0,0,0,0,0);
    m_opCycles["cli"] = MOSOperandCycle("cli",2,0,0,0,0,0);
    m_opCycles["sei"] = MOSOperandCycle("sei",2,0,0,0,0,0);
    m_opCycles["clc"] = MOSOperandCycle("clc",2,0,0,0,0,0);

    m_opCycles["lsr"] = MOSOperandCycle("lsr",2,2,6,7,5,6);
    m_opCycles["asl"] = MOSOperandCycle("asl",2,2,6,7,5,6);
    m_opCycles["ror"] = MOSOperandCycle("ror",2,2,6,7,5,6);
    m_opCycles["rol"] = MOSOperandCycle("rol",2,2,6,7,5,6);

    m_opCycles["org"] = MOSOperandCycle("org",0,0,0,0,0,0);
    m_opCycles["nop"] = MOSOperandCycle("nop",2,0,0,0,0,0);
    m_opCycles["sed"] = MOSOperandCycle("sed",2,0,0,0,0,0);
    m_opCycles["cld"] = MOSOperandCycle("cld",2,0,0,0,0,0);

    m_opCycles["sbc"] = MOSOperandCycle("sbc",0,2,4,4,3,5);
    m_opCycles["adc"] = MOSOperandCycle("adc",0,2,4,4,3,5);


    m_opCycles["ldy"] = MOSOperandCycle("ldy",0,2,4,4,3,0);
    m_opCycles["ldx"] = MOSOperandCycle("ldx",0,2,4,4,3,0);
    m_opCycles["cpy"] = MOSOperandCycle("cpy",0,2,4,4,3,0);
    m_opCycles["cpx"] = MOSOperandCycle("cpx",0,2,4,4,3,0);

    m_opCycles["cmp"] = MOSOperandCycle("cmp",0,2,4,4,3,5);
    m_opCycles["jmp"] = MOSOperandCycle("jmp",0,0,3,3,0,5);
    m_opCycles["jsr"] = MOSOperandCycle("jsr",0,0,6,6,0,0);
    m_opCycles["rts"] = MOSOperandCycle("rts",6,0,0,6,0,0);
    m_opCycles["rti"] = MOSOperandCycle("rti",6,0,0,6,0,0);
    m_opCycles["beq"] = MOSOperandCycle("beq",0,0,2,0,0,0);
    m_opCycles["bcs"] = MOSOperandCycle("bcs",0,0,2,0,0,0);
    m_opCycles["bcc"] = MOSOperandCycle("bcc",0,0,2,0,0,0);
    m_opCycles["bne"] = MOSOperandCycle("bne",0,0,2,0,0,0);
    m_opCycles["bpl"] = MOSOperandCycle("bpl",0,0,2,0,0,0);

    m_opCycles["inc"] = MOSOperandCycle("inc",0,0,6,7,5,6);
    m_opCycles["dec"] = MOSOperandCycle("dec",0,0,6,7,5,6);




}

