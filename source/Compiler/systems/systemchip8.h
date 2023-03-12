#ifndef SYSTEMCHIP8_H
#define SYSTEMCHIP8_H

#include "source/Compiler/systems/abstractsystem.h"
#include "source/messages.h"


class SystemChip8 : public AbstractSystem
{
public:
    SystemChip8(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);

    void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab) override;
    void PostProcess(QString& text, QString file, QString currentDir) override;

    QString getTripePointerType() override {
        return "uint16";
    }

    bool useZeroPagePointers() override { return true;}

    bool AllowPointerInStructs() override {return false;}

    virtual void DefaultValues() override;

    virtual QString getEmulatorName() { return "internal";}
    void applyEmulatorParameters(QStringList& params, QString debugFile, QString baseFile, CIniFile* pini) override;


    TokenType::Type getSystemPointerArrayType() override {
        return TokenType::INTEGER;
    }

    void PrepareInitialAssembler(Assembler* as) override;


};

#endif // SYSTEMMOS6502_H
