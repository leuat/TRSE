#ifndef M68000_H
#define M68000_H


#include "source/Compiler/assembler/assembler.h"


class AsmM68000 : public Assembler
{
public:
    AsmM68000();

    QString byte = "dc.b";
    QString word = "dc.w";
    QString llong = "dc.l";


    void Connect() override {}

    void Program(QString name, QString vicParam) override;
    void EndProgram() override;


    void DeclareArray(QString name, QString type, int count, QStringList data, QString pos) override;
    void DeclareVariable(QString name, QString type, QString initval) override;

    void BinOP(TokenType::Type t) override;

};

#endif // M68000_H
