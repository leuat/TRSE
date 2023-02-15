#include "compilerchip8.h"


CompilerChip8::CompilerChip8(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni) : Compiler(ini, pIni) {

}

void CompilerChip8::InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system)
{
    m_assembler = QSharedPointer<AsmJDH8>(new AsmJDH8());//
    m_codeGen = QSharedPointer<CodeGenChip8>(new CodeGenChip8());
    m_codeGen->m_outputLineNumbers = false;

//    m_assembler->Asm("BALLE");



    m_assembler->m_symTab->m_constants = m_parser.m_symTab->m_constants;
//    m_assembler->WriteConstants();

    // Init default stuff

}

void CompilerChip8::Connect()
{
    m_assembler->m_source<<m_parser.m_parserAppendix;
    m_assembler->Connect();
    m_assembler->EndMemoryBlock();
    for (QString&s :m_assembler->m_source) {
        s = s.replace("$","0x");
    }

}

bool CompilerChip8::SetupMemoryAnalyzer(QString filename, Orgasm *orgAsm)
{
    return true;
}
