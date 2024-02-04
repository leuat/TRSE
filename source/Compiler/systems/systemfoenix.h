#ifndef SYSTEMFOENIX_H
#define SYSTEMFOENIX_H

#include "system65c816.h"

class SystemFoenix : public System65C816
{
public:
    SystemFoenix(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : System65C816(settings, proj) {
        m_allowedGlobalTypeFlags << "compressed"<<"pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"invert";
        m_allowedProcedureTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"invert" <<"global" <<"stack";
        m_supportsExomizer = true;
        m_registers << "_a"<<"_x" <<"_y" <<"_ax" <<"_ay" <<"_xy";
        m_canRunAsmFiles = true;
        m_allowClasses = true; // EXPERIMENTAL
        m_processor = WDC65C02;
        float x = 1.8;
        m_systemColor = QColor(70*x,50*x,120*x);
        DefaultValues();
        m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"Zero pages",0,0x00FF));
        m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0x0100,0x01FF));
        m_system = FOENIX;


    }

    void DefaultValues() override
    {
        m_ignoreSys = false;
        m_startAddress=getDefaultBasicAddress();
        m_programStartAddress = m_startAddress;
    }

//    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;

    int getDefaultBasicAddress() override {
        return 0x2000;
    };


    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("foenix_emulator");
    }

    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
//        params  <<"-besure" <<"-prgmode" <<"65"<< "-prg" <<filename+".prg";
        params  <<filename+".bin@"+Util::numToHex(m_startAddress).remove("$") << "boot@"+Util::numToHex(m_startAddress).remove("$");
#ifdef __APPLE__
        QString out;
        StartProcess("killall", QStringList()<<"-9" <<"jr256",out);
#endif

    }

    void PrepareInitialAssembler(Assembler* as) override;
    void Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab) override;

};
#endif
