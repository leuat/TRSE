#include "compiler65c816.h"


void Compiler65C816::InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system)
{
    m_assembler = QSharedPointer<Asm6502>(new Asm6502());
    m_assembler->m_zbyte = 0x10;
    m_codeGen = QSharedPointer<CodeGen6502>(new CodeGen6502());
    Init6502Assembler();
    LabelStack::m_labelCount = 0;
    m_assembler->m_zbyte = 0x300;

    m_assembler->byte=".byte";
    m_assembler->word=".word";
    m_assembler->llong=".long";


}

void Compiler65C816::Init6502Assembler()
{
    m_assembler->m_startInsertAssembler << m_parser.m_initAssembler;
    //    qDebug() << m_parser.m_initAssembler;
    m_assembler->m_defines = m_parser.m_preprocessorDefines;




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

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::SNES)
        Syntax::s.m_ignoreSys = true;

}

void Compiler65C816::Connect()
{
//    m_assembler->EndMemoryBlock();

 /*  m_assembler->m_symTab->m_constants = m_parser.m_symTab->m_constants;
    m_assembler->WriteConstants();
*/
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::SNES) {
        m_assembler->StartMemoryBlock("0");
        m_assembler->IncludeFile(":resources/code/snes/init.asm");
    }
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::MEGA65) {
        m_assembler->StartMemoryBlock("0");
        m_assembler->IncludeFile(":resources/code/mega65/init.asm");
    }

    m_assembler->Connect();

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::SNES) {
        m_assembler->m_currentBlock = nullptr;
    }

    for (QString key : m_assembler->m_banks.keys()) {
        QSharedPointer<Appendix> app = m_assembler->m_banks[key];
        QString rom = QString::number(Util::NumberFromStringHex(key));
        app->m_source.insert(0,".segment \"RODATA"+rom+"\"");
        m_assembler->m_source << app->m_source;

    }



    if (m_ini->getdouble("post_optimize")==1.0) {
        emit EmitTick("<br>Optimising pass: ");
        m_assembler->m_totalOptimizedLines = 0;
        for (int i=0;i<4;i++) {
            emit EmitTick(" ["+QString::number(i+1)+"]");
            m_assembler->Optimise(*m_projectIni); }
    }

    CleanupCycleLinenumbers("", m_assembler->m_cycles, m_assembler->m_cyclesOut);
    CleanupCycleLinenumbers("",m_assembler->m_blockCycles,m_assembler->m_blockCyclesOut);


    CleanupBlockLinenumbers();
}

