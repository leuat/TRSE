#ifndef SYSTEMTIKI100_H
#define SYSTEMTIKI100_H

#include "systemz80.h"

class SystemTiki100 : public SystemZ80
{
public:
    SystemTiki100(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemZ80(settings, proj) {
        m_processor = Z80;
        m_system = TIKI100;

        m_startAddress = 0x0100;
        m_programStartAddress = 0x0100;
    }



    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab);
    virtual void PostProcess(QString& text, QString file, QString currentDir);
};

#endif // SYSTEMTIKI100_H
