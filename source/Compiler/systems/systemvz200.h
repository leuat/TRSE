#ifndef SYSTEMVZ200_H
#define SYSTEMVZ200_H


#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/util.h"
#include "source/Compiler/systems/systemz80.h"

class SystemVZ200 : public SystemZ80
{
public:
    SystemVZ200(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);
    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
    virtual void PostProcess(QString& text, QString file, QString currentDir) override;
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("vz200_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
        params<< "-f"<<filename+".vz";

    }

};

#endif // SYSTEMVZ200_H
