#ifndef SYSTEMAPPLEII_H
#define SYSTEMAPPLEII_H

#include "systemmos6502.h"

class SystemAppleII : public SystemMOS6502
{
public:
    SystemAppleII(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) ;
    void DefaultValues() override;

    int getDefaultBasicAddress() override {
        return 0x803;
    };

    void PostProcess(QString &text, QString file, QString currentDir) override;

    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("appleii_emulator");
    }

    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {

        QString emu = getEmulatorName();
        QString fn = filename + ".do";
        if (QFile::exists(fn))
            fn = QFileInfo(fn).absoluteFilePath();

        if (emu.toLower().contains("microm8")) {
            params = QStringList() <<"-launch" <<fn;
        }

    }


};


#endif // SYSTEMAPPLEII_H
