#include "compilerS1C88.h"


void CompilerS1C88::InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system)
{
    m_assembler = QSharedPointer<AsmZ80>(new AsmZ80());//
    m_codeGen = QSharedPointer<CodeGenZ80>(new CodeGenZ80());
    m_codeGen->m_outputLineNumbers = false;

    if (m_projectIni->getdouble("override_target_settings")==1) {
        Syntax::s.m_currentSystem->m_programStartAddress = Util::NumberFromStringHex(m_projectIni->getString("override_target_settings_org"));
    }

 //   m_assembler->Asm("CPU "+m_projectIni->getString("cpu_Z80_system"));
/*
    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::AMSTRADCPC ||
        Syntax::s.m_currentSystem->m_system == AbstractSystem::SPECTRUM ||
        Syntax::s.m_currentSystem->m_system == AbstractSystem::MSX
        ) {
        m_assembler->m_symTab->m_constants = m_parser.m_symTab->m_constants;
        m_assembler->WriteConstants();
    }
*/
    // Init default stuff



}

void CompilerS1C88::Connect()
{
    m_assembler->m_source<<m_parser.m_parserAppendix;
    m_assembler->Connect();

    m_assembler->EndMemoryBlock();





}

bool CompilerS1C88::SetupMemoryAnalyzer(QString filename, Orgasm* orgAsm)
{
    return true;
}

