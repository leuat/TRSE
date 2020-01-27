#ifndef ASMX86_H
#define ASMX86_H



#include "source/Compiler/assembler/assembler.h"


class AsmX86 : public Assembler
{
public:
    AsmX86();

    QString byte = "dc.b";
    QString word = "dc.w";
    QString llong = "dc.l";


    void Connect() override {}

    void Program(QString name, QString vicParam) override;
    void EndProgram() override;
    void Write(QString str, int level) override;


    void DeclareArray(QString name, QString type, int count, QStringList data, QString pos) override;
    void DeclareVariable(QString name, QString type, QString initval) override;

    void BinOP(TokenType::Type t) override;


    QString String(QStringList lst) override;
    void Label(QString s) override;


};

#endif // ASMX86_H
