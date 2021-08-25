#ifndef SYSTEMMSX_H
#define SYSTEMMSX_H

#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/util.h"
#include "source/Compiler/systems/systemz80.h"

class SystemMSX : public SystemZ80
{
public:
    SystemMSX(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);



    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
    virtual void PostProcess(QString& text, QString file, QString currentDir) override;

    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("msx_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
        params<< filename+".rom";
    }

};


#endif // SYSTEMMSX_H
