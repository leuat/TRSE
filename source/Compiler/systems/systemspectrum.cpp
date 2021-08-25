#include "systemspectrum.h"
#include "source/Compiler/syntax.h"

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

void SystemSpectrum::applyEmulatorParameters(QStringList &params, QString debugFile, QString filename, CIniFile *pini) {
    QString emu = getEmulatorName();
    if (emu.toLower().contains("retro")) {
        QString addr = QString::number(Syntax::s.m_currentSystem->m_programStartAddress,16);
        int model = pini->getdouble("spectrum_model");
        QStringList models = QStringList() <<"zx16k" << "zx48k"<<"zx128k";
        params<<"-b="+models[model]<<"-j=0x"+addr<<"-l=0x"+addr;
        params << filename+".bin";
    }

}
