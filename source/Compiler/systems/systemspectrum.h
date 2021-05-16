#ifndef SYSTEMSPECTRUM_H
#define SYSTEMSPECTRUM_H

#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/util.h"
#include "source/Compiler/systems/systemz80.h"

class SystemSpectrum : public SystemZ80
{
public:
    SystemSpectrum(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemZ80(settings, proj) {
        m_processor = Z80;
        m_system = SPECTRUM;

        m_startAddress = 0x6000;
        m_programStartAddress = 0x6000;
    }

    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab);
    virtual void PostProcess(QString& text, QString file, QString currentDir);

    QString getArkosTrackerSystemString() override {
        return "PLY_AKG_HARDWARE_SPECTRUM = 1\n";
    }

};


#endif // SYSTEMSPECTRUM_H
