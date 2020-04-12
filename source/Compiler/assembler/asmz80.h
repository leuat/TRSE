#ifndef ASMZ80_H
#define ASMZ80_H



#include "source/Compiler/assembler/assembler.h"

class AsmZ80 : public Assembler
{
public:
    AsmZ80();
    ~AsmZ80() {}

    QString byte = "db";
    QString word = "dw";
    QString llong = "dl";


    void Connect() override;

    void Program(QString name, QString vicParam) override;
    void EndProgram() override;
    void Write(QString str, int level=1) override;


    void DeclareArray(QString name, QString type, int count, QStringList data, QString pos) override;
    void DeclareVariable(QString name, QString type, QString initval, QString position) override;
    void DeclareString(QString name, QStringList initVal);

    void BinOP(TokenType::Type t) override;


    void Label(QString s) override;

    QString GetOrg(int pos) override;

    QString String(QStringList lst) override;


};

#endif // ASMX86_H
