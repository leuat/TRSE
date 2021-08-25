#ifndef SYSTEMMACOS_H
#define SYSTEMMACOS_H


#include "systemarm.h"

class SystemM1ARM : public SystemArm
{
public:
    SystemM1ARM(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);

    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
    virtual void PostProcess(QString& text, QString file, QString currentDir) override;

    QString getEmulatorName() override {
        return "open";
    }

    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
        //process.startDetached("open",QStringList()<<);
        params<<"-a"<<"Terminal"<<filename;
    }

};

#endif // SYSTEMMACOS_H
