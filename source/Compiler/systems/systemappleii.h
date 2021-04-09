#ifndef SYSTEMAPPLEII_H
#define SYSTEMAPPLEII_H

#include "systemmos6502.h"

class SystemAppleII : public SystemMOS6502
{
public:
    SystemAppleII(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) ;
    void DefaultValues() override;

    int getDefaultBasicAddress() override {
        return 0x803;
    };

    void PostProcess(QString &text, QString file, QString currentDir) override;

};


#endif // SYSTEMAPPLEII_H
