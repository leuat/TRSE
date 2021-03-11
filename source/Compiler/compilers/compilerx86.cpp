#include "compilerx86.h"


void CompilerX86::InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system)
{
    m_assembler = QSharedPointer<AsmX86>(new AsmX86());//
    m_dispatcher = QSharedPointer<ASTdispatcherX86>(new ASTdispatcherX86());

    m_assembler->Asm("CPU "+m_projectIni->getString("cpu_x86_system"));




}

void CompilerX86::Connect()
{
    m_assembler->m_source<<m_parser.m_parserAppendix;

    m_assembler->EndMemoryBlock();
    m_assembler->Connect();

}
