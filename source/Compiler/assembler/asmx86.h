#ifndef ASMX86_H
#define ASMX86_H



#include "source/Compiler/assembler/assembler.h"

class AsmX86 : public Assembler
{
public:
    AsmX86();

    QString byte = "db";
    QString word = "dw";
    QString llong = "dl";


    void Connect() override {}

    void Program(QString name, QString vicParam) override;
    void EndProgram() override;
    void Write(QString str, int level=1) override;


    void DeclareArray(QString name, QString type, int count, QStringList data, QString pos) override;
    void DeclareVariable(QString name, QString type, QString initval) override;

    void BinOP(TokenType::Type t) override;


    QString String(QStringList lst) override;
    void Label(QString s) override;



};

#endif // ASMX86_H
