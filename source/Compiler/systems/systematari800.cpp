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

}

void SystemAtari800::DefaultValues()
{
    m_startAddress = 0xA000;
    m_programStartAddress = 0xA000;
    m_ignoreSys = true;
    m_stripPrg = true;

}
