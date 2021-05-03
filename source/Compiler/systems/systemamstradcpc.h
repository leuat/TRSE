#ifndef SYSTEMAMSTRADCPC_H
#define SYSTEMAMSTRADCPC_H

#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/util.h"
#include "source/Compiler/systems/systemz80.h"

class SystemAmstradCPC : public SystemZ80
{
public:
    SystemAmstradCPC(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);



    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab);
    virtual void PostProcess(QString& text, QString file, QString currentDir);
    QString getArkosTrackerSystemString() override {
        return "PLY_AKG_HARDWARE_CPC = 1\n";
    }

};

#endif // SYSTEMAMSTRADCPC_H
