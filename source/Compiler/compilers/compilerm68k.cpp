#include "compilerm68k.h"


void CompilerM68K::InitAssemblerAndDispatcher(QSharedPointer<AbstractSystem> system)
{
    m_assembler = QSharedPointer<AsmM68000>(new AsmM68000());//
    m_dispatcher = QSharedPointer<ASTDispather68000>(new ASTDispather68000());

}

void CompilerM68K::Connect()
{
    //        m_assembler->blocks.append(m_assembler->m_chipMem);
    m_assembler->m_chipMem->m_source.insert(0," 	CNOP 0,4");
    if (!Syntax::s.m_currentSystem->m_systemParams.contains("ignoresystemheaders"))
        m_assembler->m_chipMem->m_source.insert(0,"	Section ChipRAM,Data_c");
    m_assembler->m_source <<m_assembler->m_chipMem->m_source;

}
