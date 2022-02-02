#include "compilers1c88.h"


void CompilerS1C88::InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system)
{
    m_assembler = QSharedPointer<AsmZ80>(new AsmZ80());//
    m_assembler->byte=".db";
    m_assembler->word=".dw";
    m_assembler->llong=".dl";

    m_codeGen = QSharedPointer<CodeGenS1C88>(new CodeGenS1C88());
    m_codeGen->m_outputLineNumbers = false;

    if (m_projectIni->getdouble("override_target_settings")==1) {
        Syntax::s.m_currentSystem->m_programStartAddress = Util::NumberFromStringHex(m_projectIni->getString("override_target_settings_org"));
    }

 //   m_assembler->Asm("CPU "+m_projectIni->getString("cpu_Z80_system"));

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::POKEMONMINI) {
 //       m_assembler->m_symTab->m_constants = m_parser.m_symTab->m_constants;
   //     m_assembler->WriteConstants();

        m_assembler->IncludeFile(":resources/code/pokemini/init.asm");
    }
    // Init default stuff



}

void CompilerS1C88::Connect()
{
    m_assembler->m_source<<m_parser.m_parserAppendix;

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::POKEMONMINI) {
        m_assembler->m_wram->m_source.insert(0,".orgfill $1500");
        m_assembler->m_source = m_assembler->m_wram->m_source << m_assembler->m_source;
    }

    m_assembler->Connect();

    m_assembler->EndMemoryBlock();





}

bool CompilerS1C88::SetupMemoryAnalyzer(QString filename, Orgasm* orgAsm)
{
    return true;
}

