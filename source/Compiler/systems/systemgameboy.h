#ifndef SYSTEMGAMEBOY_H
#define SYSTEMGAMEBOY_H

#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/util.h"
#include "source/Compiler/systems/systemz80.h"

class SystemGameboy : public SystemZ80
{
public:
    SystemGameboy(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemZ80(settings, proj) {
        m_processor = Z80;
        m_system = GAMEBOY;

    }



    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab);
    virtual void PostProcess(QString& text, QString file, QString currentDir);
};

#endif // SYSTEMM6800_H
