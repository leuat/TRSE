#include "systemz80.h"
#include "source/LeLib/util/lz4/lz4.h"
#include "source/OrgAsm/zorgasm.h"
void SystemZ80::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    AssembleZOrgasm(text,filename,currentDir,symTab);

}

void SystemZ80::PostProcess(QString &text, QString file, QString currentDir)
{
}

void SystemZ80::AssembleZOrgasm(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString output;


    if (m_orgAsm !=nullptr)
        delete m_orgAsm;


    m_orgAsm = new ZOrgasm();

    m_orgAsm->m_cpuFlavor = Orgasm::CPUFLAVOR_Z80;

    emit EmitTick("<br></font><font color=\"yellow\">Assembling with OrgAsm ");
    connect(m_orgAsm, SIGNAL(EmitTick(QString)), this, SLOT( AcceptDispatcherTick(QString)));
/*    if (symTab!=nullptr)
        m_orgAsm->SetupConstants(symTab);
*/
    if (symTab!=nullptr)
        m_orgAsm->m_extraSymbols = symTab->m_extraAtSymbols;

    m_orgAsm->Assemble(filename+".asm", filename+".bin");

    disconnect(m_orgAsm, SIGNAL(EmitTick(QString)), this, SLOT( AcceptDispatcherTick(QString)));
    output = m_orgAsm->m_output;
    if (m_orgAsm->m_hasOverlappingError) {
        output = m_orgAsm->error.msg;
        output +="<br><br>Line "+QString::number(m_orgAsm->error.oline.m_lineNumber+1)+ " in " +"<font color=\"orange\">"+filename+" :</font> "" : " +m_orgAsm->error.oline.m_orgLine;
        m_orgOutput = output;
        text="<font color=\"#FF6040\">Fatal error during OrgAsm assembly!</font><br>";
        text+=output;
        m_buildSuccess = false;
        return;

    }

    m_addresses = m_orgAsm->m_lineAddress;
    if (m_projectIni->getdouble("output_debug_symbols")==1.0)
        m_orgAsm->SaveSymbolsList(filename+".sym");

    text+=output;

}

QString SystemZ80::CompressFile(QString fileName)
{
    return CompressLZ4(fileName);
}
