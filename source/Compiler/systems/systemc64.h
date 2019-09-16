#ifndef SYSTEMC64_H
#define SYSTEMC64_H

#include "systemmos6502.h"

class SystemC64 : public SystemMOS6502
{
public:
    SystemC64(CIniFile* settings, CIniFile* proj) : SystemMOS6502(settings, proj) {
        m_processor = MOS6502;
        m_system = C64;

        m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"Zero pages",0,0x00FF));
        m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0x0100,0x01FF));
        m_labels.append(SystemLabel(SystemLabel::BASIC,"Basic",0x0200,0x03FF));
        m_labels.append(SystemLabel(SystemLabel::FREE,"Free",0x0800,0xBFFF));
        m_labels.append(SystemLabel(SystemLabel::SID,"SID",0xD400,0xD7FF));
        m_labels.append(SystemLabel(SystemLabel::COLOUR,"Colour ram",0xD800,0xDBFF));
        m_labels.append(SystemLabel(SystemLabel::FREE,"Free",0xE000,0xFFFE));

        m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen bank 0",0x0400,0x07FF));
        m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen bank 1",0x4400,0x47FF));
        m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen bank 2",0x8400,0x87FF));
        m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen bank 3",0xC400,0xC7FF));

        m_systemColor = QColor(80,110,80);

    }
};


class SystemX16 : public SystemMOS6502
{
public:
    SystemX16(CIniFile* settings, CIniFile* proj) : SystemMOS6502(settings, proj) {
        m_processor = MOS6502;
        m_system = X16;
        m_systemColor = QColor(100,60,40);

    }
};

#endif // SYSTEMC64_H
