#include "compilerm68k.h"


void CompilerM68K::InitAssemblerAndDispatcher(AbstractSystem *system)
{
    m_assembler = new AsmM68000();//
    m_dispatcher = new ASTDispather68000();

}

void CompilerM68K::Connect()
{
    //        m_assembler->blocks.append(m_assembler->m_chipMem);
    m_assembler->m_chipMem.m_source.insert(0," 	CNOP 0,4");
    m_assembler->m_chipMem.m_source.insert(0,"	Section ChipRAM,Data_c");
    m_assembler->m_source <<m_assembler->m_chipMem.m_source;

}
