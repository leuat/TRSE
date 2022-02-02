#ifndef SYSTEMTRS80_H
#define SYSTEMTRS80_H


#include "systemz80.h"

class SystemTRS80 : public SystemZ80
{
public:
    SystemTRS80(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);

    void Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab);

    void PostProcess(QString &text, QString file, QString currentDir);

    QString getArkosTrackerSystemString() override {
        return "";
    }
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("trs80_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override;


};


#endif // SYSTEMTRS80_H
