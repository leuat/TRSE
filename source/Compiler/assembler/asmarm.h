#ifndef ASMARM_H
#define ASMARM_H


#include "source/Compiler/assembler/assembler.h"

class AsmARM : public Assembler
{
public:
    AsmARM();
    ~AsmARM() {}
/*
    QString byte = "db";
    QString word = "dw";
    QString llong = "dl";
*/

    void Connect() override;

    void Program(QString name, QString vicParam) override;
    void EndProgram() override;
    void Write(QString str, int level=1) override;


    void DeclareArray(QString name, QString type, int count, QStringList data, QString pos) override;
    void DeclareVariable(QString name, QString type, QString initval, QString position) override;

    void BinOP(TokenType::Type t, bool clearFlag=true) override;
    int CodeSizeEstimator(QStringList& lines) override {return 0;}

    void DeclareString(QString name, QStringList initVal, QStringList flags) override;

    QString String(QStringList lst, bool term) override;
    void Label(QString s) override;

    QString GetOrg(int pos) override;

    bool DeclareClass(QString name, QString type, int count, QStringList data, QString pos) override;

    int m_curLabel = 1;
    QString NewLabel(QString s) override;

    void PopLabel(QString s) override;



};

#endif // ASMX86_H
