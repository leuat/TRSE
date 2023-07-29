#include "compilerpdp11.h"


void CompilerPDP11::InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system)
{
    m_assembler = QSharedPointer<AsmPDP11>(new AsmPDP11());//
    m_codeGen = QSharedPointer<CodeGenPDP11>(new CodeGenPDP11());
    m_codeGen->dontOutputLineNumbers();

    if (Data::data.demomode)
        Syntax::s.m_currentSystem->m_systemParams["ignoresystemheaders"]=(char)1;


//    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::ATARI520ST)
  //          m_assembler->IncludeFile(":resources/code/atari520st/init.s");

}


void CompilerPDP11::Connect()
{
    //        m_assembler->blocks.append(m_assembler->m_chipMem);
    m_assembler->Connect();
/*
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::ATARI520ST) {
            m_assembler->IncludeFile(":resources/code/atari520st/end.s");
//            m_assembler->IncludeFile(":resources/code/atari520st/poly.s");

    }
*/

/*    m_assembler->m_chipMem->m_source.insert(0," 	CNOP 0,4");
    if (!Syntax::s.m_currentSystem->m_systemParams.contains("ignoresystemheaders"))
        m_assembler->m_chipMem->m_source.insert(0,"	Section ChipRAM,Data_c");
    m_assembler->m_source <<m_assembler->m_chipMem->m_source;
*/



}
