#include "compilerx86.h"


void CompilerX86::InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system)
{
    m_assembler = QSharedPointer<AsmX86>(new AsmX86());//
    m_codeGen = QSharedPointer<CodeGenX86>(new CodeGenX86());

    m_assembler->Asm("CPU "+m_projectIni->getString("cpu_x86_system"));
    Syntax::s.m_currentSystem->m_systemParams["cpu"] = m_projectIni->getString("cpu_x86_system");

    if (m_projectIni->getdouble("override_target_settings")==1) {
        Syntax::s.m_currentSystem->m_startAddress = Util::NumberFromStringHex(m_projectIni->getString("override_target_settings_basic"));
        Syntax::s.m_currentSystem->m_programStartAddress = Util::NumberFromStringHex(m_projectIni->getString("override_target_settings_org"));
    }


}

void CompilerX86::Connect()
{
    m_assembler->m_source<<m_parser.m_parserAppendix;

    m_assembler->EndMemoryBlock();
    m_assembler->Connect();

}
