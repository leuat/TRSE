#ifndef SYSTEMTHOMSON
#define SYSTEMTHOMSON



#include "source/LeLib/util/util.h"
#include "system6809.h"
#include <QProcess>
#include "source/LeLib/util/util.h"



class SystemThomson : public System6809
{
public:
    SystemThomson(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);


    void PostProcess(QString &text, QString file, QString currentDir) override;

    QString getArkosTrackerSystemString() override {
        return "";
    }
    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("thomson_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override;
    void Createk5Tape(QString filename);


};

#endif // SYSTEMTRS80COCO_H
