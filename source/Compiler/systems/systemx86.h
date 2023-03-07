#ifndef SYSTEMX86_H
#define SYSTEMX86_H


#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/util.h"

class SystemX86 : public AbstractSystem
{
public:
    QString m_cpu;
    SystemX86(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : AbstractSystem(settings, proj) {
        m_processor = PX86;
        m_system = X86;
        m_startAddress = 0x100;
        if (m_projectIni->contains("qemu") && m_projectIni->getString("qemu").startsWith("qemu")){
            m_startAddress = 0x7c00;
            m_startAddress = 0x1000;
        }

        m_programStartAddress = m_startAddress;
        m_cpu = m_projectIni->getString("cpu_x86_system");
        m_allowedGlobalTypeFlags << "compressed"<<"pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term";
        m_allowedProcedureTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"global";
        m_registers << "_ax"<<"_bx" <<"_cx" <<"_dx";
        m_registers << "_ah"<<"_bh" <<"_ch" <<"_dh";
        m_registers << "_al"<<"_bl" <<"_cl" <<"_dl";
//        m_registers << "_es"<<"_di" <<"_ds" <<"_si";
        m_registers << "_di" <<"_si";
        m_hasVariableColorPalette = true;
        m_allowClasses = true;
        m_allowedBaseTypes<<"LONG";

        m_renameVariables << "ax" <<"bx"<<"cx"<<"dx";
        m_renameVariables << "ah" <<"bh"<<"ch"<<"dh";
        m_renameVariables << "al" <<"bl"<<"cl"<<"dl";
        m_renameVariables << "es"<<"ds"<<"si"<<"di";


        m_supportsInclusiveFor = false;

    }

    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
    virtual void PostProcess(QString& text, QString file, QString currentDir) override;
    virtual bool is8bit() override {
        return false;
    }


    QString getOutputType() {
        QString outputType = "exe";
        if (m_projectIni->contains("cpu_x86_output"))
            outputType = m_projectIni->getString("cpu_x86_output");
        return outputType;

    }

    TokenType::Type getSystemPointerArrayType() override {
        return TokenType::LONG;
    }
    virtual bool is486() override;
    virtual bool is386() override;
    virtual bool is286() override;
    virtual bool is8088() override;
    virtual bool is186() override;

    int getPointerSize() override {
        return 4;
    }
    virtual TokenType::Type getPointerType() override { return TokenType::Type::LONG;}

    virtual QString getEmulatorName() override;
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override;
    QString getCPUFlavor() override {
        return m_cpu;
    }

};


#endif // SYSTEMX86_H
