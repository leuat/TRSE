#include "systemvic20.h"

SystemVIC20::SystemVIC20(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemMOS6502(settings, proj) {
    m_processor = MOS6502;
    m_system = VIC20;

    m_systemColor = QColor(80,110,200);
    param = proj->getString("vic_memory_config");
    DefaultValues();
}

void SystemVIC20::DefaultValues()
{
    if (param=="none") {
        m_startAddress = 0x1000;
        m_programStartAddress = 0x1010;
        m_memoryType = 0;
    }
    else if (param=="3k") {
        m_startAddress = 0x0400;
        m_programStartAddress = 0x0410;
        m_memoryType = 0;
    }
    else
    {
        m_startAddress = 0x1200;
        m_programStartAddress = 0x1210;
        m_memoryType = 1;
   }
    m_labels.clear();


    if (param=="3k") {

        m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"Zero pages",0,0x00FF));
        m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0x0100,0x01FF));
        m_labels.append(SystemLabel(SystemLabel::STACK,"OS workspace",0x0200,0x03FF));
        m_labels.append(SystemLabel(SystemLabel::FREE,"Program memory",0x0400,0x1DFF));
        m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen default",0x1E00,0x1FFF));
        m_labels.append(SystemLabel(SystemLabel::UNAVAILABLE,"Block 1 8K unavailable",0x2000,0x3FFF));
        m_labels.append(SystemLabel(SystemLabel::UNAVAILABLE,"Block 2 8K unavailable",0x4000,0x5FFF));
        m_labels.append(SystemLabel(SystemLabel::UNAVAILABLE,"Block 3 8K unavailable",0x6000,0x7FFF));
        m_labels.append(SystemLabel(SystemLabel::SCREEN,"Character ROM",0x8000,0x8FFF));
        m_labels.append(SystemLabel(SystemLabel::REGISTERS,"VIC registers",0x9000,0x90FF));
        m_labels.append(SystemLabel(SystemLabel::IO,"VIA 1 - 2 registers and mirrors",0x9100,0x93FF));
        m_labels.append(SystemLabel(SystemLabel::COLOUR,"Unused colour RAM",0x9400,0x95FF));
        m_labels.append(SystemLabel(SystemLabel::COLOUR,"Colour RAM",0x9600,0x97FF));
        m_labels.append(SystemLabel(SystemLabel::IO,"IO2",0x9800,0x9BFF));
        m_labels.append(SystemLabel(SystemLabel::IO,"IO3",0x9C00,0x9FFF));
        m_labels.append(SystemLabel(SystemLabel::UNAVAILABLE,"Block 5 8K unavailable / Cartridge",0xA000,0xBFFF));
        m_labels.append(SystemLabel(SystemLabel::BASIC,"BASIC ROM",0xC000,0xDFFF));
        m_labels.append(SystemLabel(SystemLabel::KERNAL,"KERNAL ROM",0xE000,0xFFFF));

    }
    else if (param=="8k" ||
             param=="16k" ||
             param=="24k" ||
             param=="all") {

        m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"Zero pages",0,0x00FF));
        m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0x0100,0x01FF));
        m_labels.append(SystemLabel(SystemLabel::STACK,"OS workspace",0x0200,0x03FF));
        m_labels.append(SystemLabel(SystemLabel::FREE,"+3K RAM",0x0400,0x0FFF));
        m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen default",0x1000,0x11FF));
        m_labels.append(SystemLabel(SystemLabel::FREE,"Program memory",0x1200,0x1FFF));
        m_labels.append(SystemLabel(SystemLabel::FREE,"Block 1 +8K RAM",0x2000,0x3FFF));
        m_labels.append(SystemLabel(SystemLabel::FREE,"Block 2 +8K RAM",0x4000,0x5FFF));
        m_labels.append(SystemLabel(SystemLabel::FREE,"Block 3 +8K RAM",0x6000,0x7FFF));
        m_labels.append(SystemLabel(SystemLabel::SCREEN,"Character ROM",0x8000,0x8FFF));
        m_labels.append(SystemLabel(SystemLabel::REGISTERS,"VIC registers",0x9000,0x90FF));
        m_labels.append(SystemLabel(SystemLabel::IO,"VIA 1 - 2 registers and mirrors",0x9100,0x93FF));
        m_labels.append(SystemLabel(SystemLabel::COLOUR,"Colour RAM",0x9400,0x95FF));
        m_labels.append(SystemLabel(SystemLabel::COLOUR,"Unused colour RAM",0x9600,0x97FF));
        m_labels.append(SystemLabel(SystemLabel::IO,"IO2",0x9800,0x9BFF));
        m_labels.append(SystemLabel(SystemLabel::IO,"IO3",0x9C00,0x9FFF));
        m_labels.append(SystemLabel(SystemLabel::FREE,"Block 5 +8K RAM / Cartridge",0xA000,0xBFFF));
        m_labels.append(SystemLabel(SystemLabel::BASIC,"BASIC ROM",0xC000,0xDFFF));
        m_labels.append(SystemLabel(SystemLabel::KERNAL,"KERNAL ROM",0xE000,0xFFFF));

    }
    else {
        // Unexpanded
        m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"Zero pages",0,0x00FF));
        m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0x0100,0x01FF));
        m_labels.append(SystemLabel(SystemLabel::STACK,"OS workspace",0x0200,0x03FF));
        m_labels.append(SystemLabel(SystemLabel::UNAVAILABLE,"3K unavailable",0x0400,0x0FFF));
        m_labels.append(SystemLabel(SystemLabel::FREE,"Program memory",0x1000,0x1DFF));
        m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen default",0x1E00,0x1FFF));
        m_labels.append(SystemLabel(SystemLabel::UNAVAILABLE,"Block 1 8K unavailable",0x2000,0x3FFF));
        m_labels.append(SystemLabel(SystemLabel::UNAVAILABLE,"Block 2 8K unavailable",0x4000,0x5FFF));
        m_labels.append(SystemLabel(SystemLabel::UNAVAILABLE,"Block 3 8K unavailable",0x6000,0x7FFF));
        m_labels.append(SystemLabel(SystemLabel::SCREEN,"Character ROM",0x8000,0x8FFF));
        m_labels.append(SystemLabel(SystemLabel::REGISTERS,"VIC registers",0x9000,0x90FF));
        m_labels.append(SystemLabel(SystemLabel::IO,"VIA 1 - 2 registers and mirrors",0x9100,0x93FF));
        m_labels.append(SystemLabel(SystemLabel::COLOUR,"Unused colour RAM",0x9400,0x95FF));
        m_labels.append(SystemLabel(SystemLabel::COLOUR,"Colour RAM",0x9600,0x97FF));
        m_labels.append(SystemLabel(SystemLabel::IO,"IO2",0x9800,0x9BFF));
        m_labels.append(SystemLabel(SystemLabel::IO,"IO3",0x9C00,0x9FFF));
        m_labels.append(SystemLabel(SystemLabel::UNAVAILABLE,"Block 5 8K unavailable / Cartridge",0xA000,0xBFFF));
        m_labels.append(SystemLabel(SystemLabel::BASIC,"BASIC ROM",0xC000,0xDFFF));
        m_labels.append(SystemLabel(SystemLabel::KERNAL,"KERNAL ROM",0xE000,0xFFFF));

    }
    //...
    m_ignoreSys = false;
    m_stripPrg = false;

}
