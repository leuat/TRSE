#ifndef SYSTEMBBCM_H
#define SYSTEMBBCM_H

#include "systemmos6502.h"



class SystemBBCM : public SystemMOS6502
{
public:
    SystemBBCM(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);
    void PostProcess(QString &text, QString filename, QString currentDir) override;

    void DefaultValues() override;
};

#endif // SYSTEMBBCM_H
