#ifndef SYSTEMBK0010_H
#define SYSTEMBK0010_H

#include "systempdp11.h"

class SystemBK0010 : public SystemPDP11
{
public:
    QString m_currentDir;
    SystemBK0010(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);

    void PostProcess(QString &text, QString file, QString currentDir) override;
    virtual QString getEmulatorName() override {
        return "java";
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override;

};

#endif // SYSTEMBK0010_H
