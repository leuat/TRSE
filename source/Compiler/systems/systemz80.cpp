#include "systemz80.h"
#include "source/LeLib/util/lz4/lz4.h"
#include "source/OrgAsm/zorgasm.h"
void SystemZ80::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString output;
    AssembleZOrgasm(output,text,filename,currentDir,symTab);

}

void SystemZ80::PostProcess(QString &text, QString file, QString currentDir)
{
}


void SystemZ80::PerformAssembling(QString filename, QString &text,QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    bool useOrgasm = false;
    if (m_settingsIni->contains("assembler_z80"))
            useOrgasm = m_settingsIni->getString("assembler_z80")!="Pasmo";


    QString assembler = m_settingsIni->getString("pasmo");
    if (!useOrgasm && !QFile::exists(assembler)) {
        text  += "<br><font color=\"#FF6040\">Please set up a link to the PASMO assembler directory in the TRSE settings panel.</font>";
        m_buildSuccess = false;
        return;
    }
    if (QFile::exists(filename+".bin"))
        QFile::remove(filename+".bin");



    if (useOrgasm) {
        QString output="";
        AssembleZOrgasm(output,text,filename,currentDir,symTab);
  //      QProcess process;
//        StartProcess(assembler, QStringList() << filename+".asm" <<filename+"_pasmo.bin", text);

    }
    else {
        QProcess process;
        StartProcess(assembler, QStringList() << filename+".asm" <<filename+".bin", text);
    }



}

QString SystemZ80::CompressFile(QString fileName)
{
    return CompressLZ4(fileName);
}
