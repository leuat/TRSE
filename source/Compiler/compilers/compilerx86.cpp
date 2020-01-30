#include "compilerx86.h"


void CompilerX86::InitAssemblerAndDispatcher(AbstractSystem *system)
{
    m_assembler = new AsmX86();//
    m_dispatcher = new ASTDispatcherX86();
    m_assembler->Asm("CPU "+m_projectIni->getString("cpu_x86_system"));

}

void CompilerX86::Connect()
{
    m_assembler->EndMemoryBlock();
    m_assembler->Connect();

}
