#ifndef M68000_H
#define M68000_H


#include "source/Compiler/assembler/assembler.h"


class AsmM68000 : public Assembler
{
public:
    AsmM68000();
    ~AsmM68000() {}

    QString byte = "dc.b";
    QString word = "dc.w";
    QString llong = "dc.l";


    void Connect() override {}

    void Program(QString name, QString vicParam) override;
    void EndProgram() override;


    void DeclareArray(QString name, QString type, int count, QStringList data, QString pos) override;
    void DeclareVariable(QString name, QString type, QString initval, QString position) override;
    int CodeSizeEstimator(QStringList& lines) override {return 0;}

    void BinOP(TokenType::Type t) override;

    void DeclareString(QString name, QStringList initval) override;

    QString String(QStringList lst) override;

    QString GetOrg(int pos ) override;

    void StartMemoryBlock(QString pos) override {}

    void EndMemoryBlock() override {}


};

#endif // M68000_H
