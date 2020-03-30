#include "compiler6502.h"


void Compiler6502::InitAssemblerAndDispatcher(AbstractSystem *system)
{
    m_assembler = QSharedPointer<AsmMOS6502>(new AsmMOS6502());
    m_assembler->m_zbyte = 0x10;
    m_dispatcher = QSharedPointer<ASTDispather6502>(new ASTDispather6502());
    Init6502Assembler();
    LabelStack::m_labelCount = 0;
}

void Compiler6502::Connect()
{
//    m_assembler->EndMemoryBlock();
    m_assembler->Connect();
    if (m_ini->getdouble("post_optimize")==1.0)
        m_assembler->Optimise(*m_projectIni);

    CleanupCycleLinenumbers("", m_assembler->m_cycles, m_assembler->m_cyclesOut);
    CleanupCycleLinenumbers("",m_assembler->m_blockCycles,m_assembler->m_blockCyclesOut);

    CleanupBlockLinenumbers();

}

void Compiler6502::CleanupCycleLinenumbers(QString currentFile, QMap<int, int> &ocycles, QMap<int, int> &retcycles)
{

    QMap<int, int> cycles;
    int acc = 0;
    if (currentFile=="")
        for (int i: ocycles.keys()) {

            int count = ocycles[i];
            int nl = i;
            acc = 0;
            for (FilePart& fp : m_parser.m_lexer->m_includeFiles) {
                // Modify bi filepart
                if (nl>fp.m_startLine && nl<fp.m_endLine) {
                    cycles[fp.m_startLine]+=count;
                    count=0;
                    //               acc+=fp.m_startLine;
                }

                if (nl>=fp.m_endLine) {
                    //                qDebug() << fp.m_startLine << fp.m_count;
                    acc+=fp.m_count;
                    nl-=fp.m_count-1;
                }
            }
            if (count!=0)
                cycles[nl] = count;


        }
    else {
        for (int i: ocycles.keys()) {

            int count = ocycles[i];
            int nl = i;

            for (FilePart& fp : m_parser.m_lexer->m_includeFiles) {
                {
                    if (fp.m_name == currentFile)
                        if (nl>fp.m_startLineAcc && nl<fp.m_endLineAcc) {
                            cycles[nl-fp.m_startLineAcc] = count;
                        }

                }
            }


        }
    }
    retcycles.clear();
    for (int i: cycles.keys())
        if (cycles[i]!=0)
            retcycles[i] = cycles[i];
    //    m_assembler->m_cycles = cycles;
}

void Compiler6502::Init6502Assembler()
{
    m_assembler->m_startInsertAssembler << m_parser.m_initAssembler;
    //    qDebug() << m_parser.m_initAssembler;
    m_assembler->m_defines = m_parser.m_preprocessorDefines;

    m_assembler->InitZeroPointers(m_projectIni->getStringList("zeropages"),m_projectIni->getStringList("temp_zeropages"),m_projectIni->getStringList("var_zeropages"));
    m_assembler->m_zeropageScreenMemory = m_projectIni->getString("zeropage_screenmemory");
    m_assembler->m_replaceValues["@DECRUNCH_ZP1"] = m_projectIni->getString("zeropage_decrunch1");
    m_assembler->m_replaceValues["@DECRUNCH_ZP2"] = m_projectIni->getString("zeropage_decrunch2");
    m_assembler->m_replaceValues["@DECRUNCH_ZP3"] = m_projectIni->getString("zeropage_decrunch3");
    m_assembler->m_replaceValues["@DECRUNCH_ZP4"] = m_projectIni->getString("zeropage_decrunch4");

    m_assembler->m_ignoreInitialJump = m_projectIni->getdouble("ignore_initial_jump")==1.0;



    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20) {
    QStringList lst = m_projectIni->getStringList("via_zeropages");
    if (lst.count()<4)
        ErrorHandler::e.Error("VIC-20 compilation error: You need to specify 4 1-byte VIA zero page values in the project settings.",0);
    m_assembler->m_replaceValues["@VIA_ZP1"] = lst[0];
    m_assembler->m_replaceValues["@VIA_ZP2"] = lst[1];
    m_assembler->m_replaceValues["@VIA_ZP3"] = lst[2];
    m_assembler->m_replaceValues["@VIA_ZP4"] = lst[3];
    }



    m_assembler->m_internalZP =
            RegisterStack(QStringList()
                          <<m_projectIni->getString("zeropage_internal1")
                          <<m_projectIni->getString("zeropage_internal2")
                          <<m_projectIni->getString("zeropage_internal3")
                          <<m_projectIni->getString("zeropage_internal4"));


    /*    m_assembler->m_internalZP << m_projectIni->getString("zeropage_internal1");
        m_assembler->m_internalZP << m_projectIni->getString("zeropage_internal2");
        m_assembler->m_internalZP << m_projectIni->getString("zeropage_internal3");
        m_assembler->m_internalZP << m_projectIni->getString("zeropage_internal4");
    */


    if (m_projectIni->getdouble("override_target_settings")==1) {
        Syntax::s.m_currentSystem->m_startAddress = Util::NumberFromStringHex(m_projectIni->getString("override_target_settings_basic"));
        Syntax::s.m_ignoreSys = m_projectIni->getdouble("override_target_settings_sys")==1;
        Syntax::s.m_currentSystem->m_programStartAddress = Util::NumberFromStringHex(m_projectIni->getString("override_target_settings_org"));
        Syntax::s.m_stripPrg = m_projectIni->getdouble("override_target_settings_prg")==1;
        if (Syntax::s.m_ignoreSys)
            Syntax::s.m_currentSystem->m_startAddress = Syntax::s.m_currentSystem->m_programStartAddress;
    } else {
        Syntax::s.m_ignoreSys = false;
        Syntax::s.m_stripPrg = false;

    }


    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::NES) {
        Syntax::s.m_currentSystem->m_programStartAddress = Util::NumberFromStringHex(m_projectIni->getString("nes_code_start"));
        Syntax::s.m_currentSystem->m_startAddress = Util::NumberFromStringHex(m_projectIni->getString("nes_code_start"));
    }


}
