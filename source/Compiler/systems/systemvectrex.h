#ifndef SYSTEMVECTREX
#define SYSTEMVECTREX



#include "source/LeLib/util/util.h"
#include "system6809.h"
#include <QProcess>
#include "source/LeLib/util/util.h"



class SystemVectrex : public System6809
{
public:
    SystemVectrex(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);


    void PostProcess(QString &text, QString file, QString currentDir) override;

    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("vectrex_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override;


};

#endif // SYSTEMTRS80COCO_H
