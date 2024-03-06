#ifndef SYSTEMPRIMO_H
#define SYSTEMPRIMO_H



#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/util.h"
#include "source/Compiler/systems/systemz80.h"

class SystemPrimo : public SystemZ80
{
public:
    SystemPrimo(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);



    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("primo_emulator");
    }
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override;


    void Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab) override;



};


#endif
