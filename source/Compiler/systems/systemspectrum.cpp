#include "systemspectrum.h"

void SystemSpectrum::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString output;
    int time = timer.elapsed();
    int codeEnd = 0;


    output+="<br>";

    PerformAssembling(filename,text,currentDir,symTab);

    if (!QFile::exists(filename+".bin")) {
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
