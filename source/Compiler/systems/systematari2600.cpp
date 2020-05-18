#include "systematari2600.h"


SystemAtari2600::SystemAtari2600(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj):SystemMOS6502(settings, proj)
{
    m_processor = MOS6502;
    m_system = ATARI2600;
    DefaultValues();

}

void SystemAtari2600::DefaultValues()
{
    m_startAddress = 0xF000;
    m_programStartAddress = 0xF000;
    m_ignoreSys = true;
    m_stripPrg = true;

}
