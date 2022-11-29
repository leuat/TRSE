#ifndef SYSTEMC128_H
#define SYSTEMC128_H

#include "systemmos6502.h"

class SystemC128 : public SystemMOS6502
{
public:
    SystemC128(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemMOS6502(settings, proj) {
        m_processor = MOS6502;
        m_system = C128;
        m_systemColor = QColor(20,100,80);
        m_startAddress =getDefaultBasicAddress();
        m_programStartAddress = 0x1C10;
//        m_memorySize = 65536*2;


        m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"Zero pages",0,0x00FF));
        m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0x0100,0x01FF));
        m_labels.append(SystemLabel(SystemLabel::BASIC,"Basic",0x0200,0x03FF));
        m_labels.append(SystemLabel(SystemLabel::FREE,"Free",0x0800,0xBFFF));
        m_labels.append(SystemLabel(SystemLabel::SID,"VIC",0xD000,0xD3FF));
        m_labels.append(SystemLabel(SystemLabel::SID,"SID",0xD400,0xD7FF));
        m_labels.append(SystemLabel(SystemLabel::COLOUR,"Colour ram",0xD800,0xDBFF));
        m_labels.append(SystemLabel(SystemLabel::IO,"CIA IO",0xDC00,0xDFFF));
        m_labels.append(SystemLabel(SystemLabel::FREE,"Free",0xE000,0xFFFE));

/*
        Parameter 1: $D000-$DFFF IO (0) or RAM (1)
        Parameter 2: $4000-$7FFF System ROM (0) or RAM (1)
        Parameter 3: $8000-$BFFF System ROM (0) or RAM (1)
        Parameter 4: $C000-$FFFF System ROM (0) or RAM (1)
        Parameter 5: Ram bank 0 (0) or bank 1 (1)
  */
//        m_labels.append(SystemLabel(SystemLabel::KERNAL,"Kernal ROM",0xE000,0xFFFE));

        m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen bank 0",0x0400,0x07FF));
        m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen bank 1",0x4400,0x47FF));
        m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen bank 2",0x8400,0x87FF));
        m_labels.append(SystemLabel(SystemLabel::SCREEN,"Screen bank 3",0xC400,0xC7FF));


    }
    int getDefaultBasicAddress() override {
        return 0x1C01;
    };
    bool isCommodoreSystem() override  {return true;}
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("c128_emulator");
    }

    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
        applyEmulatorParametersVICE(params, debugFile, filename);
    }


};
#endif // SYSTEMC128_H
