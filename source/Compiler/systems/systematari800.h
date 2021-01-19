#ifndef SYSTEMATARI800_H
#define SYSTEMATARI800_H

#include "systemmos6502.h"



class SystemAtari800 : public SystemMOS6502
{
public:
    SystemAtari800(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);
    void PostProcess(QString &text, QString filename, QString currentDir) override;

    void DefaultValues() override;
};

#endif // SYSTEMBBCM_H
