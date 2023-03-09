#include "systemtiki100.h"
#include "source/LeLib/util/tikidisk.h"

extern "C" {
    void addTikiFile (char *efn, unsigned char user);
}

void SystemTiki100::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString output;
    int time = timer.elapsed();
    int codeEnd = 0;


    PerformAssembling(filename,text,currentDir,symTab);
    if (!m_buildSuccess)
        return;

    if (m_projectIni->getdouble("exomizer_toggle")==1)
        PackProgram(filename,0x300, 0x300, text, currentDir, symTab,":resources/code/tiki100/unpack.asm");



    Util::CopyFile(filename+".bin",filename+".com");

    if (!QFile::exists(filename+".com")) {
        text  += "<br><font color=\"#FFFF00\">Error during assembly : please check source assembly for errors.</font>";
        text+=output;
        m_buildSuccess = false;
        return;
    }
    output+="<br>";
    QString disk = currentDir +  "disk1.dsk";
    Util::CopyFile(":resources/bin/tiki/disk.dsk", disk);

    Util::CopyFile(":resources/bin/tiki/tiki.rom", "tiki.rom");
    InitTikiDisk((char*)disk.toStdString().c_str());
    addTikiFile((char*)(QString(filename+".com").toStdString().c_str()),0);
    disk = currentDir + "disk.dsk";
    SaveTikiDisk((char*)disk.toStdString().c_str());


    int assembleTime = timer.elapsed()- time;
    time = timer.elapsed();

    //    qDebug() << "*********" << output;

    text+=output;

}

void SystemTiki100::PostProcess(QString &text, QString file, QString currentDir)
{

}
