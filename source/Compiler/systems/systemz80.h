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
        m_renameVariables << "a" <<"b"<<"c"<<"d"<<"i"<<"af"<<"bc"<<"de"<<"hl"<<"ix"<<"iy"<<"sp"<<"h"<<"l"<<"p"<<"r"<<"pc"<<"e"<<"sz"<<"pnc" <<"z"<<"nz"<<"nc" <<"po"<<"pe"<<"m";
//        m_registers <<"_a" <<"_b"<<"_c"<<"_d"<<"_af"<<"_bc"<<"_de"<<"_hl"<<"_ix"<<"_iy"<<"_h"<<"_l";
    }
    // Performs the full assembling + post processing stuff
    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
    // Generic post processing
    virtual void PostProcess(QString& text, QString file, QString currentDir) override;
    // Perform assembling of 1 file - using either orgasm or pasmo
    void PerformAssembling(QString filename, QString &text,QString currentDir, QSharedPointer<SymbolTable> symTab);

    virtual QString getTripePointerType() override {
        return "uint16";
    }
    TokenType::Type getSystemPointerArrayType() override {
        return TokenType::INTEGER;
    }

    QString CompressFile(QString fileName) override;

    virtual int getCPUFlavorint() override {
        return 3; // z80
    }


};

#endif

