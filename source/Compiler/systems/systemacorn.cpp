#include "systemacorn.h"
SystemAcorn::SystemAcorn(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemMOS6502(settings, proj)
{
    m_processor = MOS6502;
    m_system = ACORN;
    DefaultValues();

    m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"Zero pages",0,0x00FF));
    m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0x0100,0x01FF));

    m_labels.append(SystemLabel(SystemLabel::SCREEN,"VRAM",0x8000,0x9800));

    m_labels.append(SystemLabel(SystemLabel::KERNAL,"Filesystem ROM",0xE000,0xEFFF));
    m_labels.append(SystemLabel(SystemLabel::KERNAL,"IO",0xB000,0xBFFF));
    m_labels.append(SystemLabel(SystemLabel::KERNAL,"MOS",0xF000,0xFFFF));
    m_labels.append(SystemLabel(SystemLabel::KERNAL,"BASIC",0xC000,0xCFFF));

    m_systemColor = QColor(120,100,80);

}

void SystemAcorn::PostProcess(QString &text, QString filename, QString currentDir)
{
    auto d = Util::loadBinaryFile(filename+".prg");
    QByteArray h;
    /*
00-15, 16 bytes for program name
16-17, Loadaddress (lb,hb)
18-19, Execution address (lb,hb)
20-21, Filelength without header (lb,hb)
    */


    h.resize(16);
    h[0]='M';
    h[1]='E';
    h[2]='N';
    h[3]='U';
    h[4]='0';
    Util::WriteInt16LH(h,m_startAddress);
    Util::WriteInt16LH(h,m_startAddress);
    Util::WriteInt16LH(h,d.size());

    h.append(d);
//    Util::SaveByteArray(h,filename+".atm");
    Util::SaveByteArray(h,filename+".atm");
    QString dir = QFileInfo(getEmulatorName()).absoluteDir().path()+QDir::separator()+"MMC";
    if (!QDir(dir).exists())
     QDir().mkdir(dir);
    Util::CopyFile(filename+".atm",dir+QDir::separator()+"MENU");

}


void SystemAcorn::DefaultValues()
{
    m_startAddress = 0x1000;
    m_programStartAddress = 0x1000;
    m_ignoreSys = true;
    m_stripPrg = true;
    m_memorySize = 65536;
    m_requireEmulatorWorkingDirectory = true;

}

