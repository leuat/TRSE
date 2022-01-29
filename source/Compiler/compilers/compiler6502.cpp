#include "compiler6502.h"
#include "../codegen/codegen_tripe.h"
#include "../assembler/asmTripe.h"

void Compiler6502::InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system)
{
    if (m_projectIni->getdouble("use_tripe")==1.0) {
        m_codeGen = QSharedPointer<CodeGenTRIPE>(new CodeGenTRIPE());
        m_assembler = QSharedPointer<AsmTripe>(new AsmTripe());
    }
    else {
        m_codeGen = QSharedPointer<CodeGen6502>(new CodeGen6502());
        m_assembler = QSharedPointer<Asm6502>(new Asm6502());
    }
    m_assembler->m_zbyte = 0x10;

    Init6502Assembler();
    LabelStack::m_labelCount = 0;
}

void Compiler6502::Connect()
{
//    m_assembler->EndMemoryBlock();

 /*  m_assembler->m_symTab->m_constants = m_parser.m_symTab->m_constants;
    m_assembler->WriteConstants();
*/
    m_assembler->Connect();

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::ATARI2600)
        m_assembler->IncludeFile(":resources/code/atari2600/end.asm");



    if (m_ini->getdouble("post_optimize")==1.0) {
        emit EmitTick("<br>Optimising pass: ");
        m_assembler->m_totalOptimizedLines = 0;
        for (int i=0;i<4;i++) {
            emit EmitTick(" ["+QString::number(i+1)+"]");
            m_assembler->Optimise(*m_projectIni); }
    }

    CleanupCycleLinenumbers("", m_assembler->m_cycles, m_assembler->m_cyclesOut);
    CleanupCycleLinenumbers("",m_assembler->m_blockCycles,m_assembler->m_blockCyclesOut);

//    qDebug() << m_assembler->m_addresses.keys();

    CleanupBlockLinenumbers();
/*    for (QString&s : m_assembler->m_source) {
        s = s.replace("sta(","sta (");
        s = s.replace("lda(","lda (");
    }
    */
}

void Compiler6502::CleanupCycleLinenumbers(QString currentFile, QMap<int, int> &ocycles, QMap<int, int> &retcycles, bool isCycles)
{

    QMap<int, int> cycles;
//    int acc = 0;
    if (currentFile=="")
        for (int i: ocycles.keys()) {

            int count = ocycles[i];
            int nl = i;
  //          acc = 0;
            for (FilePart& fp : m_parser.m_lexer->m_includeFiles) {
                // Modify bi filepart
                if (nl>fp.m_startLine && nl<fp.m_endLine) {
                    if (isCycles)
                        cycles[fp.m_startLine]+=count;
                    count=0;
                    //               acc+=fp.m_startLine;
                }

                if (nl>=fp.m_endLine) {
                    //                qDebug() << fp.m_startLine << fp.m_count;
    //                acc+=fp.m_count;
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


    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::ATARI2600)
        m_assembler->IncludeFile(":resources/code/atari2600/init.asm");



    m_assembler->InitZeroPointers(m_projectIni->getStringList("zeropages"),m_projectIni->getStringList("temp_zeropages"),m_projectIni->getStringList("var_zeropages"));
    m_assembler->m_zeropageScreenMemory = m_projectIni->getString("zeropage_screenmemory");
    m_assembler->m_zeropageColorMemory = m_projectIni->getString("zeropage_colormemory");
    m_assembler->m_replaceValues["@DECRUNCH_ZP1"] = m_projectIni->getString("zeropage_decrunch1");
    m_assembler->m_replaceValues["@DECRUNCH_ZP2"] = m_projectIni->getString("zeropage_decrunch2");
    m_assembler->m_replaceValues["@DECRUNCH_ZP3"] = m_projectIni->getString("zeropage_decrunch3");
    m_assembler->m_replaceValues["@DECRUNCH_ZP4"] = m_projectIni->getString("zeropage_decrunch4");

/*    qDebug() << m_projectIni->contains("ignore_initial_jump");
    for (CItem i : m_projectIni->items)
        qDebug() << i.name;
*/
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
        Syntax::s.m_currentSystem->m_stripPrg = m_projectIni->getdouble("override_target_settings_prg")==1;
        if (Syntax::s.m_ignoreSys)
            Syntax::s.m_currentSystem->m_startAddress = Syntax::s.m_currentSystem->m_programStartAddress;
    } else {
        Syntax::s.m_currentSystem->DefaultValues();
        Syntax::s.m_ignoreSys = Syntax::s.m_currentSystem->m_ignoreSys;
 //       Syntax::s.m_stripPrg = false;

    }
   /* if (Syntax::s.m_currentSystem->m_system==AbstractSystem::ATARI2600 ||
          Syntax::s.m_currentSystem->m_system==AbstractSystem::ATARI2600  ) {
        Syntax::s.m_ignoreSys = true;
        Syntax::s.m_stripPrg = true;

    }*/

/*    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::MEGA65) {
        Syntax::s.m_ignoreSys = true;
        Syntax::s.m_stripPrg = false;

    }
*/
    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::NES) {
        Syntax::s.m_currentSystem->m_programStartAddress = Util::NumberFromStringHex(m_projectIni->getString("nes_code_start"));
        Syntax::s.m_currentSystem->m_startAddress = Util::NumberFromStringHex(m_projectIni->getString("nes_code_start"));
    }

    if (Syntax::s.m_currentSystem->isCommodoreSystem() && !Syntax::s.m_ignoreSys)
        Syntax::s.m_currentSystem->m_startAddress = Syntax::s.m_currentSystem->getDefaultBasicAddress();


}


int Compiler6502::FindEndSymbol(Orgasm &orgasm)
{
    //    QStringList output = QString(out).split("\n");
    for (QString s : orgasm.m_symbols.keys()) {
        if (s.toLower().contains("endsymbol")) {
            return orgasm.m_symbols[s];
        }
    }
    return 0;
}

QVector<int> Compiler6502::FindBlockEndSymbols(Orgasm &orgasm)
{
    QVector<int> m_blockEndSymbols;

    m_blockEndSymbols.clear();
    for (QString s : orgasm.m_symbols.keys()) {
        if (s.toLower().contains("endblock")) {
            QString spl = s;
            spl = spl.toLower().simplified().split("block")[1];
            //            bool ok;
            int i= orgasm.m_symbols[s];//.toInt(&ok, 16);
            //qDebug() << "FOUND endblock : " << s << Util::numToHex(i);
            m_blockEndSymbols.append(i);
        }
    }
    return m_blockEndSymbols;
}

void Compiler6502::ConnectBlockSymbols(QVector<int> &blockEndSymbols)
{
    for (int sym : blockEndSymbols) {
        int winner = 0xFFFF;
        QSharedPointer<MemoryBlock> winnerBlock=nullptr;

        for (QSharedPointer<MemoryBlock> mb: m_assembler->blocks) {
            //            if (mb->m_type==MemoryBlock::CODE &&  sym>mb->m_start)
            if (sym>mb->m_start)
                if (sym-mb->m_start<winner) {
                    winner = sym-mb->m_start;
                    winnerBlock  =mb;
                }
        }
        if (winnerBlock!=nullptr) {
            winnerBlock->m_end = sym;
            //     qDebug() << Util::numToHex(sym) << " " << winnerBlock->Type();
        }
    }


}
