#ifndef SYSTEMAMSTRADCPC464_H
#define SYSTEMAMSTRADCPC464_H

#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/util.h"
#include "source/Compiler/systems/systemz80.h"

class SystemAmstradCPC464 : public SystemZ80
{
public:
    SystemAmstradCPC464(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);



    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab);
    virtual void PostProcess(QString& text, QString file, QString currentDir);
};

#endif // SYSTEMAMSTRADCPC464_H
