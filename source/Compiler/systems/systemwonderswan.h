#ifndef SYSTEMWONDERSWAN_H
#define SYSTEMWONDERSWAN_H

#include "systemx86.h"

class SystemWonderswan : public SystemX86
{
public:
    QString m_cpu;

    SystemWonderswan(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);

//    virtual void PostProcess(QString& text, QString file, QString currentDir) override;


    virtual QString getEmulatorName() override;
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override;
    QString getCPUFlavor() override {
        return m_cpu;
    }
    void PostProcess(QString &text, QString file, QString currentDir) override;

};


#endif // SYSTEMX86_H
