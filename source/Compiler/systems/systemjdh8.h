#ifndef SYSTEMJDH8_H
#define SYSTEMJDH8_H

#include "abstractsystem.h"


class SystemJDH8 : public AbstractSystem
{
public:
    SystemJDH8(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);

    void DefaultValues();
    void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
    void PostProcess(QString& text, QString file, QString currentDir) override;
    void applyEmulatorParameters(QStringList &params, QString debugFile, QString filename, CIniFile *pini);
    QString getEmulatorName() override {
        return m_settingsIni->getString("jdh8_directory") + "/bin/emu";
    }

};

#endif // SYSTEMJDH8_H
