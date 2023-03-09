#include "compilerm68k.h"


void CompilerM68K::InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system)
{
    m_assembler = QSharedPointer<Asm68000>(new Asm68000());//
    m_codeGen = QSharedPointer<CodeGen68k>(new CodeGen68k());
    m_codeGen->dontOutputLineNumbers();

    if (Data::data.demomode)
        Syntax::s.m_currentSystem->m_systemParams["ignoresystemheaders"]=(char)1;

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::AMIGA) {
        if (!Syntax::s.m_currentSystem->m_systemParams.contains("ignoresystemheaders"))
            m_assembler->IncludeFile(":resources/code/amiga/init.s");
        else
            m_assembler->IncludeFile(":resources/code/amiga/init_stripped.s");
    }

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::ATARI520ST)
            m_assembler->IncludeFile(":resources/code/atari520st/init.s");

}

void Asm68000::EndProgram()
{

}

void CompilerM68K::Connect()
{
    //        m_assembler->blocks.append(m_assembler->m_chipMem);
    m_assembler->Connect();

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::AMIGA) {
        if (!Syntax::s.m_currentSystem->m_systemParams.contains("ignoresystemheaders"))
            m_assembler->IncludeFile(":resources/code/amiga/end.s");
        else
            m_assembler->IncludeFile(":resources/code/amiga/end_stripped.s");
    }

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::ATARI520ST) {
            m_assembler->IncludeFile(":resources/code/atari520st/end.s");
//            m_assembler->IncludeFile(":resources/code/atari520st/poly.s");

    }


    m_assembler->m_chipMem->m_source.insert(0," 	CNOP 0,4");
    if (!Syntax::s.m_currentSystem->m_systemParams.contains("ignoresystemheaders"))
        m_assembler->m_chipMem->m_source.insert(0,"	Section ChipRAM,Data_c");
    m_assembler->m_source <<m_assembler->m_chipMem->m_source;




}
