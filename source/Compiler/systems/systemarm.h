#ifndef SYSTEMARM_H
#define SYSTEMARM_H

#include "abstractsystem.h"

class SystemArm : public AbstractSystem
{
public:
    SystemArm(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);
    virtual QString getEmulatorName() override {
        return "open";
    }

};

#endif // SYSTEMARM_H
