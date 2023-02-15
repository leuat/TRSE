#ifndef SYSTEMCHIP8_H
#define SYSTEMCHIP8_H

#include "source/Compiler/systems/abstractsystem.h"
#include "source/messages.h"


class SystemChip8 : public AbstractSystem
{
public:
    SystemChip8(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : AbstractSystem(settings, proj) {
        m_allowedGlobalTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"invert";
        m_allowedProcedureTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"invert" <<"global";
        m_supportsExomizer = true;
     //   m_registers "_y" <<"_ax" <<"_ay" <<"_xy";
        m_canRunAsmFiles = false;
        m_allowClasses = false; // EXPERIMENTAL


    }

    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
    virtual void PostProcess(QString& text, QString file, QString currentDir) override;

    virtual QString getTripePointerType() override {
        return "uint16";
    }

    virtual bool useZeroPagePointers() override { return true;}

    virtual bool AllowPointerInStructs() override {return false;}

    void DefaultValues() override;


    TokenType::Type getSystemPointerArrayType() override {
        return TokenType::INTEGER;
    }

    virtual void PrepareInitialAssembler(Assembler* as) override;


};

#endif // SYSTEMMOS6502_H
