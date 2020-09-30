#ifndef SYSTEMZ80_H
#define SYSTEMZ80_H

#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/util.h"

class SystemZ80 : public AbstractSystem
{
public:
    SystemZ80(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : AbstractSystem(settings, proj) {
        m_allowedGlobalTypeFlags << "compressed" <<"pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term";
        m_allowedProcedureTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"global";
        m_exomizerName ="Compress executable";

    }

    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab);
    virtual void PostProcess(QString& text, QString file, QString currentDir);

    QString CompressFile(QString fileName) override;
};

#endif

