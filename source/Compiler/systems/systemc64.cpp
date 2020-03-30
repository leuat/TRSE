#include "systemc64.h"


SystemC64::SystemC64(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemMOS6502(settings, proj) {
    m_processor = MOS6502;
    m_system = C64;
    DefaultValues();

    m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"Zero pages",0,0x00FF));
    m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0x0100,0x01FF));
    m_labels.append(SystemLabel(SystemLabel::BASIC,"Basic",0x0200,0x03FF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"Free",0x0800,0xBFFF));
    m_labels.append(SystemLabel(SystemLabel::SID,"VIC",0xD000,0xD3FF));
    m_labels.append(SystemLabel(SystemLabel::SID,"SID",0xD400,0xD7FF));
    m_labels.append(SystemLabel(SystemLabel::COLOUR,"Colour ram",0xD800,0xDBFF));
    m_labels.append(SystemLabel(SystemLabel::IO,"CIA IO",0xDC00,0xDFFF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"Free",0xE000,0xFFFE));
    m_labels.append(SystemLabel(SystemLabel::KERNAL,"Kernal ROM",0xE000,0xFFFE));

    m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen bank 0",0x0400,0x07FF));
    m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen bank 1",0x4400,0x47FF));
    m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen bank 2",0x8400,0x87FF));
    m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen bank 3",0xC400,0xC7FF));

    m_systemColor = QColor(80,110,80);

}

void SystemC64::DefaultValues()
{
    m_startAddress = 0x0800;
    m_programStartAddress = 0x0810;
    m_ignoreSys = false;
    m_stripPrg = false;
}
