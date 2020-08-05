#include "compilerz80.h"


void CompilerZ80::InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system)
{
    m_assembler = QSharedPointer<AsmZ80>(new AsmZ80());//
    m_dispatcher = QSharedPointer<ASTdispatcherZ80>(new ASTdispatcherZ80());
    m_dispatcher->m_outputLineNumbers = false;

    if (m_projectIni->getdouble("override_target_settings")==1) {
        Syntax::s.m_currentSystem->m_programStartAddress = Util::NumberFromStringHex(m_projectIni->getString("override_target_settings_org"));
    }


//    m_assembler->Asm("CPU "+m_projectIni->getString("cpu_Z80_system"));

    if (Syntax::s.m_currentSystem->m_system != AbstractSystem::COLECO)
        m_assembler->Asm(" org "+Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress));

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::TIKI100) {
        m_assembler->Asm(" jp $8000");
        m_assembler->Asm(" org $8000");

    }

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::COLECO) {
        m_assembler->IncludeFile(":resources/code/coleco/header.asm");
//        m_assembler->Asm(" org 8000h");


    }



}

void CompilerZ80::Connect()
{
    m_assembler->m_source<<m_parser.m_parserAppendix;

    m_assembler->EndMemoryBlock();
    m_assembler->Connect();

    if (Syntax::s.m_currentSystem->m_system != AbstractSystem::COLECO)
        m_assembler->Asm("end "+Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress));



    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::COLECO) {

        m_assembler->IncludeFile(":resources/code/coleco/footer.asm");
        m_assembler->m_source <<m_assembler->m_wram->m_source;
    }




}

