#ifndef SYSTEM6809_H
#define SYSTEM6809_H

#include "abstractsystem.h"

class System6809 : public AbstractSystem
{
public:
    System6809(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);
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

    virtual int addressBusBits() override {
        return 24;
    }
    virtual bool is6809() override { return true;}

};

#endif // SYSTEM6809_H
