#ifndef SYSTEMMACOS_H
#define SYSTEMMACOS_H


#include "systemarm.h"

class SystemM1ARM : public SystemArm
{
public:
    SystemM1ARM(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);

    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab);
    virtual void PostProcess(QString& text, QString file, QString currentDir);

};

#endif // SYSTEMMACOS_H
