#ifndef SYSTEMTIM_H
#define SYSTEMTIM_H



#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/util.h"
#include "source/Compiler/systems/systemz80.h"

class SystemTIM : public SystemZ80
{
public:
    SystemTIM(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);


    QString m_workDirName = "disk";

//    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
//    virtual void PostProcess(QString& text, QString file, QString currentDir) override;
    QString getArkosTrackerSystemString() override {
        return "";
    }
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("tim_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override;

    void SetupDisk(QString projectPath);
    void CleanupDisk(QString projectPath);

    void Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab) override;

    void ExtraEmulatorCommands() override;
    void SendKeyCommand(QString keys);


};


#endif // SYSTEMTIM_H
