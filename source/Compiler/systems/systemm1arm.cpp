#include "systemm1arm.h"
#include <QProcess>

SystemM1ARM::SystemM1ARM(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemArm(settings,proj)
{
    m_system = M1ARM;
}

void SystemM1ARM::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
//    qDebug() << "A";
    bool ok = GenericAssemble("/usr/bin/as",
                              QStringList()<<filename+".asm"<<"-o"<<filename+".o"<<"-arch"<<"arm64" ,
                              "You need to be on an apple m1 mac in order to build this project.",text);

    if (!ok)
        return;

    //    ld -macosx_version_min 11.0.0 -o HelloWorld HelloWorld.o -lSystem -syslibroot `xcrun -sdk macosx –show-sdk-path` -e _start -arch arm64
    QString syslibroot;
    StartProcess("xcrun", QStringList() <<"-sdk"<<"macosx"<<"-show-sdk-path",syslibroot,true,currentDir);
    syslibroot.remove("\n");
    StartProcess("/usr/bin/ld",
                 QStringList() <<"-macosx_version_min"<<"11.0.0"
                <<"-o"<<filename<<filename+".o"
                <<"-lSystem"<< "-syslibroot" <<syslibroot <<"-e" <<"_start" <<"-arch" <<"arm64"

                ,text,true,currentDir);

    if (text.toLower().contains("undefined") || text.contains("error")) {
        m_buildSuccess = false;

     }
}

void SystemM1ARM::PostProcess(QString &text, QString file, QString currentDir)
{

}
