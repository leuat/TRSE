#ifndef SYSTEMARM_H
#define SYSTEMARM_H

#include "abstractsystem.h"

class SystemArm : public AbstractSystem
{
public:
    SystemArm(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);

};

#endif // SYSTEMARM_H
