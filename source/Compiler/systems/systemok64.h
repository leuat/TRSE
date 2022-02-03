#ifndef SYSTEMOK64_H
#define SYSTEMOK64_H

#include "abstractsystem.h"
#include "systemmos6502.h"

class SystemOK64 : public SystemMOS6502
{
public:
    SystemOK64(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemMOS6502(settings, proj) {
        m_processor = MOS6502;
        m_system = OK64;
        m_startAddress = 0x03FF;
        m_programStartAddress = 0x03FF;
        m_memorySize = 65536 + 65536*16;
        m_hasVariableColorPalette = true;
    }
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("ok64_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
        if (QFile::exists(debugFile))
            params<<"-moncommands"<<debugFile;
        params << filename+".prg";

    }
    virtual int addressBusBits() override {
        return 24;
    }

};

#endif // SYSTEMOK64_H
