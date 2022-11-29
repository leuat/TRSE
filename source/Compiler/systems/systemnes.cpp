#include "systemnes.h"

SystemNES::SystemNES(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemMOS6502(settings, proj) {
    m_processor = MOS6502;
    m_system = NES;
    DefaultValues();

    m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"Zero page",0,0x00FF));
    m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0x0100,0x01FF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"RAM",0x0200,0x07FF));
    m_labels.append(SystemLabel(SystemLabel::UNAVAILABLE,"RAM Mirror",0x0800,0x1FFF));
    m_labels.append(SystemLabel(SystemLabel::IO,"PPU",0x2000,0x2007));
    m_labels.append(SystemLabel(SystemLabel::UNAVAILABLE,"PPU Mirror",0x2008,0x3FFFF)); // That's a lot of space for 8 bytes of IO
    m_labels.append(SystemLabel(SystemLabel::IO,"APU",0x4000,0x4017));
    m_labels.append(SystemLabel(SystemLabel::IO,"CPU TEST",0x4018,0x401F));
    // The following layout depends on the cartridge/mapper used
    m_labels.append(SystemLabel(SystemLabel::ROM,"Expansion ROM",0x4020,0x5FFF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"Cartridge SRAM",0x6000,0x7FFF)); // Not always available
    m_labels.append(SystemLabel(SystemLabel::ROM,"PRG ROM",0x8000,0xFFFF));
    // Of course, FFFA-FFFF contains NMI/IRQ/Reset vectors
    // The PPU has its own memory layout (including the CHRROM, etc)
    m_hasVariableColorPalette = true;
    m_systemColor = QColor(138, 137, 136); // Dark gray NES
}

void SystemNES::DefaultValues() {
    m_startAddress = 0xCA00;
    m_programStartAddress = 0xCA00;
    m_ignoreSys = true;
    Syntax::s.m_ignoreSys = true;
    m_stripPrg = false;
    useZByte = true;

}
