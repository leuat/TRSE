#ifndef SYSTEMCHIP8_H
#define SYSTEMCHIP8_H

#include "source/Compiler/systems/abstractsystem.h"
#include "source/messages.h"


class SystemChip8 : public AbstractSystem
{
public:
    SystemChip8(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);

    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
    virtual void PostProcess(QString& text, QString file, QString currentDir) override;

    virtual QString getTripePointerType() override {
        return "uint16";
    }

    virtual bool useZeroPagePointers() override { return true;}

    virtual bool AllowPointerInStructs() override {return false;}

    void DefaultValues() override;

    virtual QString getEmulatorName() { return "chip8";}
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString baseFile, CIniFile* pini) override;


    TokenType::Type getSystemPointerArrayType() override {
        return TokenType::INTEGER;
    }

    virtual void PrepareInitialAssembler(Assembler* as) override;


};

#endif // SYSTEMMOS6502_H
