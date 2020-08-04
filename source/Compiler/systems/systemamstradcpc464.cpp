#include "systemamstradcpc464.h"


SystemAmstradCPC464::SystemAmstradCPC464(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj): SystemZ80(settings, proj) {
    m_processor = Z80;
    m_system = AMSTRADCPC464;

/*    m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen",0x8000,0x8FFF));
    m_labels.append(SystemLabel(SystemLabel::SCREEN,"Video ram",0x9000,0x9FFF));
    m_labels.append(SystemLabel(SystemLabel::ROM,"BANK 0 ROM",0x0,0x3FFF));
    m_labels.append(SystemLabel(SystemLabel::ROM,"BANK 1-128 ROM",0x4000,0x7FFF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"Work RAM Bank 0 (sprites etc)",0xC000,0xCFFF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"Work RAM Bank 1",0xD000,0xDFFF));
    m_labels.append(SystemLabel(SystemLabel::KERNAL,"Sprite attribute table",0xFE00, 0xFE9F));
    m_labels.append(SystemLabel(SystemLabel::KERNAL,"IO",0xFF00, 0xFF7f));
    m_labels.append(SystemLabel(SystemLabel::FREE,"HRAM",0xFF80, 0xFFFF));
*/
    m_startAddress = 0x6000;
    m_programStartAddress = 0x6000;

}

void SystemAmstradCPC464::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString output;
    int time = timer.elapsed();
    int codeEnd = 0;

    output+="<br>";
    QString assembler = m_settingsIni->getString("pasmo");
    if (!QFile::exists(assembler)) {
        text  += "<br><font color=\"#FF6040\">Please set up a link to the PASMO assembler directory in the TRSE settings panel.</font>";
        return;
    }
/*    if (!QFile::exists(m_settingsIni->getString("cpcdisk_location"))) {
        text  += "<br><font color=\"#FF6040\">Please set up a link to the CPCDiskXP disk utility TRSE 'Utilities' settings panel.</font>";
        return;

    }
*/
    if (QFile::exists(filename+".bin"))
        QFile::remove(filename+".bin");

  /*  if (QFile::exists(filename+".dsk"))
        QFile::remove(filename+".dsk");
*/
    QProcess process;
    QStringList params;
    StartProcess(assembler, QStringList() << filename+".asm" <<filename+".bin", output);

    if (!QFile::exists(filename+".bin")) {
        text  += "<br><font color=\"#FFFF00\">Error during assembly : please check source assembly for errors.</font>";
        text+=output;
        m_buildSuccess = false;
        return;
    }
    output+="<br>";
/*
    // Temporary solution
    QString fn = filename;
    fn = fn.remove(currentDir).remove("/");
    QProcess* pr = new QProcess();
#ifdef _WIN32
    QStringList ps = QStringList()                 << "-File"<< ( fn+".bin")
                                                   <<"-AddAmsdosHeader"<< "6000"
                                                   <<"-AddToNewDsk"<< fn+".dsk";

    pr->setWorkingDirectory(currentDir);
    pr->start(m_settingsIni->getString("cpcdisk_location"), ps);
#else

    QStringList ps = QStringList()                  <<m_settingsIni->getString("cpcdisk_location")
                                                   << "-File"<< ( fn+".bin")
                                                   <<"-AddAmsdosHeader"<< "6000"
                                                   <<"-AddToNewDsk"<< fn+".dsk";

    pr->setWorkingDirectory(currentDir);
    pr->start("wine", ps);
#endif
    pr->waitForFinished();
*/
//    qDebug() <<     fn+".dsk";
  //  Util::CopyFile(fn+".dsk",fn+"2.dsk");
    //Util::CopyFile(fn+"2.dsk",fn+".dsk");

//    int assembleTime = timer.elapsed()- time;
    time = timer.elapsed();

    //    qDebug() << "*********" << output;

    text+=output;

}

void SystemAmstradCPC464::PostProcess(QString &text, QString file, QString currentDir)
{

}
