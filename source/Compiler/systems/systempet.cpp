#include "systempet.h"

SystemPET::SystemPET(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemMOS6502(settings, proj) {
    m_processor = MOS6502;
    m_system = PET;
    DefaultValues();

    m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"Zero pages",0,0x00FF));
    m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0x0100,0x01FF));
    m_labels.append(SystemLabel(SystemLabel::BASIC,"OS workspace",0x0200,0x03FF));

    m_labels.append(SystemLabel(SystemLabel::FREE,"Free",0x0400,0x7FFF));
    m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen 40 Col",0x8000,0x83FF));
    m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen 80 Col",0x8400,0x87FF));
    m_labels.append(SystemLabel(SystemLabel::COLOUR,"Colour 40 Col",0x8800,0x8BFF));
    m_labels.append(SystemLabel(SystemLabel::COLOUR,"Colour 80 Col",0x8C00,0x8FFF));

    m_labels.append(SystemLabel(SystemLabel::ROM,"Option ROM #1",0x9000,0x9FFF));
    m_labels.append(SystemLabel(SystemLabel::ROM,"Option ROM #2",0xA000,0xAFFF));

    m_labels.append(SystemLabel(SystemLabel::BASIC,"Basic 4",0xB000,0xBFFF));
    m_labels.append(SystemLabel(SystemLabel::BASIC,"Basic 4",0xC000,0xCFFF));
    m_labels.append(SystemLabel(SystemLabel::BASIC,"Basic 4",0xD000,0xDFFF));

    m_labels.append(SystemLabel(SystemLabel::ROM,"Editor ROM #2",0xE000,0xE7FF));
    m_labels.append(SystemLabel(SystemLabel::IO,"IO",0xE800,0xE8FF));
    m_labels.append(SystemLabel(SystemLabel::ROM,"Extended Editor ROM #2",0xE900,0xEFFF));

    m_labels.append(SystemLabel(SystemLabel::KERNAL,"Kernal ROM",0xF000,0xFFFF));

    m_systemColor = QColor(129,138,204);

}

void SystemPET::DefaultValues()
{
    m_startAddress = 0x0400;
    m_programStartAddress = 0x0410;
    m_ignoreSys = false;
    m_stripPrg = false;
}

void SystemPET::InitSystemPreprocessors(QMap<QString, QString> &defines)  {
    defines["PET_MODEL"] = m_projectIni->getString("petmodel");
}
