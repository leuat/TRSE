#ifndef SYSTEMPCW_H
#define SYSTEMPCW_H



#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/util.h"
#include "source/Compiler/systems/systemtim.h"

class SystemPCW : public SystemTIM
{
public:
    SystemPCW(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);


    QString m_workDirName = "disk";

//    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
//    virtual void PostProcess(QString& text, QString file, QString currentDir) override;
    QString getArkosTrackerSystemString() override {
        return "";
    }
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("pcw_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override;


//    void Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab) override;

    void ExtraEmulatorCommands() override;


};


#endif // SYSTEMTIM_H
