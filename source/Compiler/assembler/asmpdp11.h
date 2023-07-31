#ifndef ASMPDP11_H
#define ASMPDP11_H

#include "source/Compiler/assembler/assembler.h"

class AsmPDP11 : public Assembler
{
public:
    AsmPDP11();
    ~AsmPDP11() {}

    QString byte = "dc.b";
    QString word = "dc.w";
    QString llong = "dc.l";


    void Connect() override;

    void Program(QString name, QString vicParam) override;
//    void EndProgram() override;

    bool DeclareClass(QString name, QString type, int count, QStringList data, QString pos) override;

    void DeclareArray(QString name, QString type, int count, QStringList data, QString pos) override;
    void DeclareVariable(QString name, QString type, QString initval, QString position) override;
    int CodeSizeEstimator(QStringList& lines) override {return 0;}

    void BinOP(TokenType::Type t, bool clearFlag=true) override;

    void Label(QString s) override;

    void DeclareString(QString name, QStringList initval, QStringList flags) override;

    QString String(QStringList lst, bool term) override;

    QString GetOrg(int pos ) override;

    void StartMemoryBlock(QString pos) override {}

    void EndMemoryBlock() override {}



};

#endif // M68000_H
