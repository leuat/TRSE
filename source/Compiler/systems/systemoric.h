#ifndef SYSTEMORIC_H
#define SYSTEMORIC_H

#include "systemmos6502.h"

class SystemOric : public SystemMOS6502
{
public:
    SystemOric(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) ;
    void DefaultValues() override;

    int getDefaultBasicAddress() override {
        return 0x500;
    };
    bool isCommodoreSystem() override  {return false;}

    void Assemble(QString& text, QString filename, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;


    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("oric_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {


        QString fn = filename + ".prg";
        if (QFile::exists(fn))
            fn = QFileInfo(fn).absoluteFilePath();

        params = QStringList() <<"-m" <<"atmos"<<"-t"<<fn;

    }


};

#endif // SYSTEMORIC_H
