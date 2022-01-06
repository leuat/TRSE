#include "compilerjdh8.h"


CompilerJDH8::CompilerJDH8(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni) : Compiler(ini, pIni) {

}

void CompilerJDH8::InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system)
{
    m_assembler = QSharedPointer<AsmJDH8>(new AsmJDH8());//
    m_codeGen = QSharedPointer<CodeGenJDH8>(new CodeGenJDH8());
    m_codeGen->m_outputLineNumbers = false;

//    m_assembler->Asm("BALLE");


    m_assembler->Asm("@include \"os/arch.asm \"");
    m_assembler->Asm("@include \"os/oscall.asm \"");

    m_assembler->Asm("@org ADDR_RAM");


    m_assembler->m_symTab->m_constants = m_parser.m_symTab->m_constants;
//    m_assembler->WriteConstants();

    // Init default stuff

}

void CompilerJDH8::Connect()
{
    m_assembler->m_source<<m_parser.m_parserAppendix;
    m_assembler->Connect();
    m_assembler->EndMemoryBlock();
    for (QString&s :m_assembler->m_source) {
        s = s.replace("$","0x");
    }

}

bool CompilerJDH8::SetupMemoryAnalyzer(QString filename, Orgasm *orgAsm)
{
    return true;
}
