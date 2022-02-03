#ifndef SYSTEMTRS80COCO_H
#define SYSTEMTRS80COCO_H



#include "source/LeLib/util/util.h"
#include "system6809.h"
#include <QProcess>
#include "source/LeLib/util/util.h"



class SystemTRS80CoCo : public System6809
{
public:
    SystemTRS80CoCo(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);

    void Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab);

    void PostProcess(QString &text, QString file, QString currentDir);

    QString getArkosTrackerSystemString() override {
        return "";
    }
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("pokemonmini_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override;


};

#endif // SYSTEMTRS80COCO_H
