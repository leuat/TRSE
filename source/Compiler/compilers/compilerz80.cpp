#include "compilerz80.h"


void CompilerZ80::InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system)
{
    m_assembler = QSharedPointer<AsmZ80>(new AsmZ80());//
    m_dispatcher = QSharedPointer<ASTdispatcherZ80>(new ASTdispatcherZ80());
    m_dispatcher->m_outputLineNumbers = false;

    if (m_projectIni->getdouble("override_target_settings")==1) {
        Syntax::s.m_currentSystem->m_programStartAddress = Util::NumberFromStringHex(m_projectIni->getString("override_target_settings_org"));
    }

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::AMSTRADCPC464)
    if (m_projectIni->getdouble("exomizer_toggle")==1.0)
        Syntax::s.m_currentSystem->m_programStartAddress = 0x200; // Unpack address


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
    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::AMSTRADCPC464) {
        m_assembler->m_symTab->m_constants = m_parser.m_symTab->m_constants;
        m_assembler->WriteConstants();
    }


    // Init default stuff



}

void CompilerZ80::Connect()
{
    m_assembler->m_source<<m_parser.m_parserAppendix;

    m_assembler->EndMemoryBlock();
    m_assembler->Connect();

    m_assembler->IncludeFile(":resources/code/Z80/memcpy.asm");


    if (Syntax::s.m_currentSystem->m_system != AbstractSystem::COLECO)
        m_assembler->Asm("end "+Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress));



    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::COLECO) {

        m_assembler->IncludeFile(":resources/code/coleco/footer.asm");
        m_assembler->m_source <<m_assembler->m_wram->m_source;
    }

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::AMSTRADCPC464) {
//        m_assembler->IncludeFile(":resources/code/amstrad/playerakg.asm");
    }




}

