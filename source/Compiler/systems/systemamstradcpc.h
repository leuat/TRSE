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



    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
    virtual void PostProcess(QString& text, QString file, QString currentDir) override;
    QString getArkosTrackerSystemString() override {
        return "PLY_AKG_HARDWARE_CPC = 1\n";
    }
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("amstradcpc_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
        if (QFile::exists(debugFile))
            params<<"-s"<<debugFile;
        params << "-i"<<filename+".bin";

        int num = m_programStartAddress;
        if (pini->getdouble("exomizer_toggle")==1)
            num = 0x4000; /// Always start at 0x4000
        params << "-o" << "0x"+QString::number(num,16);
        QString amstradcpc_model = pini->getString("amstradcpc_model");
        QMap<QString, QString> caprice32_models = {
            { "464", "0" },
            { "664", "1" },
            { "6128", "2" },
            { "6128 Plus", "3" },
        };
        amstradcpc_model = caprice32_models[amstradcpc_model];
        if (amstradcpc_model != "") {
            params << "-O" << "system.model=" + amstradcpc_model;
        }
        QString amstradcpc_options = pini->getString("amstradcpc_options");
        params << amstradcpc_options.split(' ');
        //        qDebug() <<"CURRADDR" <<"0x"+QString::number(Syntax::s.m_currentSystem->m_programStartAddress,16);

        m_requireEmulatorWorkingDirectory = true;
    }



};

#endif // SYSTEMAMSTRADCPC_H
