#include "systemvic20.h"

SystemVIC20::SystemVIC20(CIniFile *settings, CIniFile *proj) : SystemMOS6502(settings, proj) {
    m_processor = MOS6502;
    m_system = VIC20;

    // See SystemC64 for implementation details
    if (proj->getString("vic_memory_config")=="3k") {
        m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"Zero pages",0,0x00FF));
        m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0x0100,0x01FF));
        m_labels.append(SystemLabel(SystemLabel::BASIC,"Basic",0x0200,0x03FF));
        m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen bank 0",0x0400,0x07FF));
    }
    if (proj->getString("vic_memory_config")=="8k") {

    }
    //...
}
