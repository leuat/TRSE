#ifndef SYSTEMMEGA65_H
#define SYSTEMMEGA65_H

#include "system65c816.h"

class SystemMega65 : public System65C816
{
public:
    SystemMega65(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : System65C816(settings, proj) {
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
        m_system = MEGA65;


    }

//    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;

    virtual QString getEmulatorName() override {
        return m_settingsIni->getString("mega65_emulator");
    }

    void applyEmulatorParameters(QStringList& params, QString debugFile, QString filename, CIniFile* pini) override {
        params  <<"-besure" <<"-prgmode" <<"65"<< "-prg" <<filename+".prg";
    }


};

#endif // SYSTEMMEGA65_H
