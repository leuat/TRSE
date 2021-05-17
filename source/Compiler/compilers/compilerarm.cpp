#include "compilerarm.h"


void CompilerArm::InitAssemblerAnddispatcher(QSharedPointer<AbstractSystem> system)
{
    m_assembler = QSharedPointer<AsmARM>(new AsmARM());//
    m_codeGen = QSharedPointer<CodeGenARM>(new CodeGenARM());
    m_codeGen->m_outputLineNumbers = false;

//    m_assembler->Asm("CPU "+m_projectIni->getString("cpu_Z80_system"));


}

void CompilerArm::Connect()
{
    m_assembler->Connect();


}

bool CompilerArm::SetupMemoryAnalyzer(QString filename, Orgasm *orgAsm)
{

}
