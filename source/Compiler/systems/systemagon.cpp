#include "systemagon.h"

#include "source/LeLib/util/lz4/lz4.h"
#include "source/Compiler/errorhandler.h"

SystemAgon::SystemAgon(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj): SystemZ80(settings, proj) {
    m_processor = Z80;
    m_system = AGON;

    m_startAddress = 0x040000;
    m_programStartAddress = 0x040000;
    m_supportsExomizer = false;


/*    m_labels.append(SystemLabel(SystemLabel::BASIC,"Basic",0x0000,0x3FFF));
    m_labels.append(SystemLabel(SystemLabel::KERNAL,"ROM Cart",0x4000,0x67FF));
    m_labels.append(SystemLabel(SystemLabel::IO,"Input/Output",0x6800,0x6FFF));
    m_labels.append(SystemLabel(SystemLabel::SCREEN,"Video RAM",0x7000,0x77FF));
    m_labels.append(SystemLabel(SystemLabel::BASIC,"Basic Workspace",0x7800,0x7AFF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"Free",0x7B00,0xFFFF));*/
    m_systemColor = QColor(0,64,64);

    m_requireEmulatorWorkingDirectory = true;


}

void SystemAgon::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString output;
    int time = timer.elapsed();

    output+="<br>";
    QFileInfo fn(filename+".asm");

//    PerformAssembling(fn.fileName().remove(".asm"),text,currentDir,symTab);
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


    time = timer.elapsed();


    text+=output;

}

void SystemAgon::PostProcess(QString &text, QString file, QString currentDir)
{
    QDir d = QFileInfo(m_settingsIni->getString("agon_emulator")).absoluteDir();
    Util::CopyFile(file+".bin",d.absolutePath()+"/sdcard/program.bin");

    auto autoexec = d.absolutePath()+"/sdcard/autoexec.txt";

/*    if (QFile::exists(autoexec)) {
        Util::CopyFile(autoexec, autoexec+"_keep.txt");
    }*/
    QString a = "load program.bin\nrun\n";
    Util::SaveTextFile(autoexec, a);

    // Add vz header
/*    auto d = Util::loadBinaryFile(file+".bin");
    QByteArray h;
    h.append("VZF0");
    h.append("TRSE");
    for (int i=0;i<13;i++)
        h.append((char)0x0);
    h.append(0xf1);
    h.append((char)0x0);
    h.append(0x7b);
    h.append(d);
    Util::SaveByteArray(h,file+".vz");
*/

}
