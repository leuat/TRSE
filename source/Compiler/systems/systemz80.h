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
        m_renameVariables << "a" <<"b"<<"c"<<"d"<<"i"<<"af"<<"bc"<<"de"<<"hl"<<"ix"<<"iy";
    }
    // Performs the full assembling + post processing stuff
    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
    // Generic post processing
    virtual void PostProcess(QString& text, QString file, QString currentDir) override;
    // Assembles using Zorgasm only
    virtual void AssembleZOrgasm(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab);
    // Perform assembling of 1 file - using either orgasm or pasmo
    void PerformAssembling(QString filename, QString &text,QString currentDir, QSharedPointer<SymbolTable> symTab);

    QString CompressFile(QString fileName) override;
};

#endif

