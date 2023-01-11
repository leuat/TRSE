#include "systemtim.h"
#include <QThread>

SystemTIM::SystemTIM(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj): SystemZ80(settings, proj)
{
    m_processor = Z80;
    m_system = TIM;

    m_startAddress = 0x0100;
    m_programStartAddress = 0x0100;
    m_supportsExomizer = true;


    //   m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"System values",0,0x00FF));
    /* m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0xBF00,0xBFFF));
    m_labels.append(SystemLabel(SystemLabel::BASIC,"Basic",0x0200,0x3FFF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"Free",0x4000,0xAFFF));
    m_labels.append(SystemLabel(SystemLabel::VIC,"Video memory",0xC000,0xFFFF));
    m_labels.append(SystemLabel(SystemLabel::SID,"Firmware",0xB000,0xBEFF));
*/
    m_systemColor = QColor(90,45,20);
    m_requireEmulatorWorkingDirectory = true;

}

void SystemTIM::SetupDisk(QString projectPath)
{
    QString path = projectPath + "/" + m_workDirName;

    QDir dir(path);
    if (dir.exists())
        dir.removeRecursively();

    QDir().mkdir(path);
    QDir().mkdir(path+"/floppy");

    Util::CopyFile(":resources/bin/tim_floppy/diskdefs",path+"/diskdefs");
    Util::CopyFile(":resources/bin/tim_floppy/empty_9600.img",path+"/empty_9600.img");

    Util::CopyFile(":resources/bin/tim_floppy/floppy/emu.com",path+"/floppy/emu.com");
    Util::CopyFile(":resources/bin/tim_floppy/floppy/ldr.com",path+"/floppy/ldr.com");
    Util::CopyFile(":resources/bin/tim_floppy/floppy/mdsk.com",path+"/floppy/mdsk.com");
    Util::CopyFile(":resources/bin/tim_floppy/floppy/names.ndr",path+"/floppy/names.ndr");
    Util::CopyFile(":resources/bin/tim_floppy/floppy/start.com",path+"/floppy/start.com");
    Util::CopyFile(":resources/bin/tim_floppy/floppy/tim.fcp",path+"/floppy/tim.fcp");
    Util::CopyFile(":resources/bin/tim_floppy/floppy/tim.iop",path+"/floppy/tim.iop");
    Util::CopyFile(":resources/bin/tim_floppy/floppy/tim.rcp",path+"/floppy/tim.rcp");
    Util::CopyFile(":resources/bin/tim_floppy/floppy/tim.z3t",path+"/floppy/tim.z3t");
    Util::CopyFile(":resources/bin/tim_floppy/floppy/valias.com",path+"/floppy/valias.com");
    Util::CopyFile(":resources/bin/tim_floppy/floppy/wheel.com",path+"/floppy/wheel.com");
    Util::CopyFile(":resources/bin/tim_floppy/floppy/zcpr3.sys",path+"/floppy/zcpr3.sys");


}

void SystemTIM::CleanupDisk(QString projectPath)
{
    QString path = projectPath + "/" + m_workDirName;

    QDir dir(path);
    if (dir.exists())
        dir.removeRecursively();

}

void SystemTIM::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString output;
    int time = timer.elapsed();

    output+="<br>";

    QString cpmtools = m_settingsIni->getString("cpmtools_directory");


    QDir dir(cpmtools);
    if (!QFile::exists(cpmtools+"/cpmcp")) {
        text  += "<br><font color=\"#FF0000\">You need to set up a link to the CPMTools directory in the TRSE settings.</font>";
        text+=output;
        m_buildSuccess = false;
        return;

    }

    PerformAssembling(filename,text,currentDir,symTab);

    if (!QFile::exists(filename+".bin")) {
        text  += "<br><font color=\"#FFFF00\">Error during assembly : please check source assembly for errors.</font>";
        text+=output;
        m_buildSuccess = false;
        return;
    }
    if (m_buildSuccess) {
        text +="<br>Assembled file size: <b>" + QString::number(QFileInfo(filename+".bin").size()) + "</b> bytes";
    }

    //output+="<br>";

    time = timer.elapsed();

    SetupDisk(currentDir);
    QString workDir = currentDir+"/"+m_workDirName+"/";

    QString dname = workDir+"empty_9600.img";
    QFile(dname).setPermissions(QFileDevice::ReadOther | QFileDevice::WriteOther);
    QFile(dname).setPermissions(QFileDevice::ReadOther | QFileDevice::WriteOther);

    QString floppyDir = workDir+"floppy/";
    Util::CopyFile(filename+".bin", floppyDir+"r.com");
    QString out;

#ifdef __linux__
    base = base.remove("x16emu");
#endif
#ifdef __APPLE__
    StartProcess("chmod",QStringList() <<"a+rw"<<dname,out,true,workDir);

#endif
    QDir directory(floppyDir);
    QStringList files = directory.entryList(QStringList() << "*.*",QDir::Files | QDir::NoDotAndDotDot);
    for (auto& f:files) {
        StartProcess(cpmtools+"/cpmcp",QStringList()
                     <<"-f" <<"tim011"
                     //    <<"\"" + floppy + "\""
                     <<dname
                     //                 <<"\""+currentDir+"/"+m_floppy+"\"*"
                     //                 <<currentDir+"/"+dname
                     //                 <<floppyDir+"*"
                     <<floppyDir+f
                     <<"0:"
                     ,out,true,workDir
                     );
    }
    //    qDebug() << out;
    QString outFile = filename+".img";
    if (QFile::exists(outFile))
        QFile::remove(outFile);

    Util::CopyFile(dname,outFile);



    CleanupDisk(currentDir);



    text+=output;

}

void SystemTIM::ExtraEmulatorCommands()
{
/*    QString t = "
    "sleep 1"
    "ID=$(xdotool search --name TIM-011)"
    "sendKey space space Scroll_Lock F11 F10"
    "sleep 2"
    "sendKey F10 r Return F10"
    "sleep 0.2"
    "sendKey F10 F11 Scroll_Lock"
            ""


*/
    //sendkeys -a "tim011" -c "<c:return> hello"
#ifdef __APPLE__
//    QThread::sleep(3);
  //  SendKeyCommand("<c:space:space>");

 #endif
}

void SystemTIM::SendKeyCommand(QString keys)
{
    auto cmd ="/opt/homebrew/sendkeys";
    if (!QFile::exists(cmd))
        return;
    QString out;
    StartProcess(cmd, QStringList() << "-a"<<"tim011"<<
                 "-c"<<cmd,
                 out,true
                 );
    qDebug() << "*** OUT:";
    qDebug() << out;
#
}

void SystemTIM::applyEmulatorParameters(QStringList &params, QString debugFile, QString filename, CIniFile *pini) {
    //    $MAME tim011 -window -v -r 720x512 -switchres -flop1 $FLOPPY.img 1>/dev/null &

//    params <<"tim011" <<"-window" <<"-v"<<"-r"<<"720x512"<<"-switchres"<<"-nothrottle" <<"-flop1"  <<filename+".img";
    params <<"tim011" <<"-window" <<"-v"<<"-r"<<"720x512"<<"-switchres" <<"-flop1"  <<filename+".img";

    m_requireEmulatorWorkingDirectory = true;
}
