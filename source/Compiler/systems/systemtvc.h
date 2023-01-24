#ifndef SYSTEMTVC_H
#define SYSTEMTVC_H



#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/util.h"
#include "source/Compiler/systems/systemz80.h"

class SystemTVC : public SystemZ80
{
public:
    SystemTVC(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);



//    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
//    virtual void PostProcess(QString& text, QString file, QString currentDir) override;
    QString getArkosTrackerSystemString() override {
        return "";
    }
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("TVC_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override;


    void Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab);



};


#endif // SYSTEMTVC_H
