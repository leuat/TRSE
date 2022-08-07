#include "systematari800.h"

SystemAtari800::SystemAtari800(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemMOS6502(settings, proj)
{
    m_processor = MOS6502;
    m_system = ATARI800;
    DefaultValues();

    m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"Zero pages",0,0x00FF));
    m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0x0100,0x01FF));

//    m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen bank 0",0x4000,0x7FFF));

    m_systemColor = QColor(50,100,160);

}

void SystemAtari800::PostProcess(QString &text, QString filename, QString currentDir)
{

    QByteArray data = Util::loadBinaryFile(filename+".prg");
    QByteArray h;
    int start = m_startAddress;
    int end = start + data.length();
    h.append(0xFF); // Binary header notification
    h.append(0xFF);
    h.append((start&0xff));
    h.append(((start>>8)&0xff));
    h.append((end&0xff));
    h.append(((end>>8)&0xff));
    if ((unsigned char)data[1]==((m_startAddress>>8)&0xff) &&(unsigned char)data[0]==((m_startAddress)&0xff))
    {
        // Strip header
        data.remove(0,2);
    }
//    h.append((len&0xff));
 //   h.append(((len>>8)&0xff));

    h.append(data);

    Util::SaveByteArray(h,filename+".xex");

}

void SystemAtari800::DefaultValues()
{
    m_startAddress = 0xA000;
    m_programStartAddress = 0xA000;
    m_ignoreSys = true;
    m_stripPrg = true;

}
