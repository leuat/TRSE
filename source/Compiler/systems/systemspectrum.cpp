#include "systemspectrum.h"

void SystemSpectrum::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString output;
    int time = timer.elapsed();
    int codeEnd = 0;


    output+="<br>";
    QString assembler = m_settingsIni->getString("pasmo");
    if (!QFile::exists(assembler)) {
        text  += "<br><font color=\"#FF6040\">Please set up a link to the PASMO assembler directory in the TRSE settings panel.</font>";
        m_buildSuccess = false;
        return;
    }

    if (QFile::exists(filename+".tap"))
        QFile::remove(filename+".tap");
    QProcess process;
    QStringList params;
    StartProcess(assembler, QStringList() << "-1" << "--tapbas" << filename+".asm" <<filename+".tap", output);

   // PerformAssembling(filename,text,currentDir,symTab);
    if (!QFile::exists(filename+".tap")) {
        text  += "<br><font color=\"#FFFF00\">Error during assembly : please check source assembly for errors.</font>";
        text+=output;
        m_buildSuccess = false;
        return;
    }
    output+="<br>";


    int assembleTime = timer.elapsed()- time;
    time = timer.elapsed();

    //    qDebug() << "*********" << output;

    text+=output;

}

void SystemSpectrum::PostProcess(QString &text, QString file, QString currentDir)
{

}
